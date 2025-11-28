#include "../../include/database/DbManager.hpp"
#include "../../include/backend/Utils.hpp"
#include <iostream>
#include <sstream>
#include <cstring>

DbManager::DbManager(const std::string& caminhoDb) 
    : db(nullptr), dbPath(caminhoDb) {
    
    if (!loadDB()) {
        std::cerr << "Erro ao conectar ao banco de dados!" << std::endl;
    }
    
    createTables();
}

DbManager::~DbManager() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DbManager::loadDB() {
    int resultado = sqlite3_open(dbPath.c_str(), &db);
    
    if (resultado != SQLITE_OK) {
        std::cerr << "Erro ao abrir banco: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    std::cout << "-> Banco de dados conectado: " << dbPath << std::endl;
    return true;
}

void DbManager::closeDB() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DbManager::verifyConnection() {
    return db != nullptr;
}

void DbManager::createTables() {
    if (!verifyConnection()) {
        std::cerr << "Não há conexão com o banco!" << std::endl;
        return;
    }
    
    // ==================== USER ====================
    std::string sqlUser = R"(
    CREATE TABLE IF NOT EXISTS usuarios (
        id INTEGER PRIMARY KEY,
        nome TEXT NOT NULL,
        email TEXT UNIQUE NOT NULL,
        phone_number TEXT,
        senha TEXT NOT NULL,
        state INTEGER DEFAULT 1,
        descricao TEXT,
        totalScore INTEGER DEFAULT 0,
        cpf TEXT UNIQUE,
        seguidores TEXT DEFAULT '',
        seguindo TEXT DEFAULT ''
    );
)";
    
    // ==================== COMPANY ====================
    std::string sqlCompany = R"(
    CREATE TABLE IF NOT EXISTS company (
        id INTEGER PRIMARY KEY,
        nome TEXT NOT NULL,
        email TEXT UNIQUE NOT NULL,
        phone_number TEXT,
        password TEXT NOT NULL,
        state INTEGER DEFAULT 1,
        descricao TEXT,
        cnpj TEXT UNIQUE NOT NULL,
        seguidores TEXT DEFAULT '',
        seguindo TEXT DEFAULT ''
    );
)";
    
    // ==================== CASES ====================
    std::string sqlGlobalCase = R"(
        CREATE TABLE IF NOT EXISTS cases_globais (
            id INTEGER PRIMARY KEY,
            nome TEXT NOT NULL,
            descricao TEXT,
            solucao TEXT,
            pontos REAL DEFAULT 0.0,
            estado INTEGER DEFAULT 1
        );
    )";
    
    // ==================== CASES POR COMPANY ====================
    std::string sqlCompanyCase = R"(
        CREATE TABLE IF NOT EXISTS cases_company (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            case_id INTEGER NOT NULL,
            company_id INTEGER NOT NULL,
            nome TEXT NOT NULL,
            descricao TEXT,
            solucao TEXT,
            pontos REAL DEFAULT 0.0,
            estado INTEGER DEFAULT 1,
            FOREIGN KEY(company_id) REFERENCES companies(id),
            UNIQUE(case_id, company_id)
        );
    )";
    
    // ==================== RANKING GLOBAL ====================
    std::string sqlGlobalranking = R"(
        CREATE TABLE IF NOT EXISTS ranking_global (
            posicao INTEGER PRIMARY KEY AUTOINCREMENT,
            usuario_id INTEGER NOT NULL,
            pontos REAL NOT NULL,
            FOREIGN KEY(usuario_id) REFERENCES usuarios(id)
        );
    )";
    
    // ==================== RANKING POR COMPANY ====================
    std::string sqlCompanyRanking = R"(
        CREATE TABLE IF NOT EXISTS ranking_company (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            company_id INTEGER NOT NULL,
            usuario_id INTEGER NOT NULL,
            pontos REAL NOT NULL,
            posicao INTEGER,
            FOREIGN KEY(company_id) REFERENCES companies(id),
            FOREIGN KEY(usuario_id) REFERENCES usuarios(id),
            UNIQUE(company_id, usuario_id)
        );
    )";
    
    // ==================== CASOS RESOLVIDOS (histórico) ====================
    std::string sqlSolvedCases = R"(
        CREATE TABLE IF NOT EXISTS solved_cases (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            usuario_id INTEGER NOT NULL,
            case_id INTEGER NOT NULL,
            data_resolucao DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(usuario_id) REFERENCES usuarios(id),
            UNIQUE(usuario_id, case_id)
        );
    )";
    
    // ==================== POSTS ====================
    std::string sqlPosts = R"(
        CREATE TABLE IF NOT EXISTS posts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            usuario_id INTEGER NOT NULL,
            autor_nome TEXT NOT NULL,
            conteudo TEXT NOT NULL,
            curtidas INTEGER DEFAULT 0,
            data_criacao DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(usuario_id) REFERENCES usuarios(id)
        );
    )";
    
    // ====================(follow) ====================
    std::string sqlRelations = R"(
        CREATE TABLE IF NOT EXISTS relacionamentos (
            seguidor TEXT NOT NULL,
            seguido TEXT NOT NULL,
            data_seguida DATETIME DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY(seguidor, seguido),
            FOREIGN KEY(seguidor) REFERENCES usuarios(id),
            FOREIGN KEY(seguido) REFERENCES usuarios(id)
        );
    )";
    
    executerSQL(sqlUser);
    executerSQL(sqlCompany);
    executerSQL(sqlGlobalCase);
    executerSQL(sqlCompanyCase);
    executerSQL(sqlGlobalranking);
    executerSQL(sqlCompanyRanking);
    executerSQL(sqlSolvedCases);
    executerSQL(sqlPosts);
    executerSQL(sqlRelations);
    
    std::cout << "-> Tabelas criadas/verificadas com sucesso!" << std::endl;
}

bool DbManager::executerSQL(const std::string& sql) {
    if (!verifyConnection()) {
        std::cerr << "Não há conexão com o banco!" << std::endl;
        return false;
    }
    
    char* errmsg = nullptr;
    int resultado = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
    
    if (resultado != SQLITE_OK) {
        std::cerr << "Erro ao executar SQL: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        return false;
    }
    
    return true;
}

// ==================== USUÁRIOS ====================

bool DbManager::saveUsers(const std::vector<User>& users) {
    if (!verifyConnection()) return false;
    
    executerSQL("DELETE FROM usuarios;");
    
    for (const auto& user : users) {
        // user é const, então precisa de const_cast para chamar versão não-const
        const auto& seguidores = const_cast<User&>(user).getFollowers();
        const auto& seguindo = const_cast<User&>(user).getFollowing();
        
        std::string seguidoresStr = "";
        for (size_t i = 0; i < seguidores.size(); ++i) {
            seguidoresStr += seguidores[i];  
            if (i < seguidores.size() - 1) seguidoresStr += ",";
        }
        
        std::string seguindoStr = "";
        for (size_t i = 0; i < seguindo.size(); ++i) {
            seguindoStr += seguindo[i];  
            if (i < seguindo.size() - 1) seguindoStr += ",";
        }
        
        std::ostringstream sql;
        sql << "INSERT INTO usuarios (id, nome, email, phone_number, senha, state, descricao, totalScore, cpf, seguidores, seguindo) "
            << "VALUES (" << user.getId() << ", '"
            << user.getName() << "', '"
            << user.getEmail() << "', '"
            << user.getPhoneNumber() << "', '"
            << user.getPassword() << "', "
            << (user.getState() ? 1 : 0) << ", '"
            << user.getDescription() << "', "
            << user.getTotalScore() << ", '"
            << user.getCpf() << "', '"
            << seguidoresStr << "', '"
            << seguindoStr << "');";
        
        if (!executerSQL(sql.str())) {
            std::cerr << "Erro ao salvar usuário: " << user.getName() << std::endl;
            return false;
        }
    }
    
    std::cout << "DB Manager -> " <<  "Usuários salvos!" << std::endl;
    return true;
}

std::vector<User> DbManager::loadUsers() {
    std::vector<User> usuarios;
    
    if (!verifyConnection()) return usuarios;
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM usuarios;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
        return usuarios;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Remover leitura do ID (coluna 0)
        // int id = sqlite3_column_int(stmt, 0);  ← REMOVIDO
        
        const char* nome = (const char*)sqlite3_column_text(stmt, 1);
        const char* email = (const char*)sqlite3_column_text(stmt, 2);
        const char* phone_number = (const char*)sqlite3_column_text(stmt, 3);
        const char* senha = (const char*)sqlite3_column_text(stmt, 4);
        int state = sqlite3_column_int(stmt, 5);
        const char* descricao = (const char*)sqlite3_column_text(stmt, 6);
        int totalScore = sqlite3_column_int(stmt, 7);
        const char* cpf = (const char*)sqlite3_column_text(stmt, 8);
        
        // Construtor SEM id
        User user(
            nome != nullptr ? nome : "",
            email != nullptr ? email : "",
            phone_number != nullptr ? phone_number : "31912345678",
            senha != nullptr ? senha : "",
            state != 0,
            descricao != nullptr ? descricao : "",
            totalScore,
            cpf != nullptr ? cpf : "00000000000"
        );
        
        // Carregar followers
        const char* seguidoresStr = (const char*)sqlite3_column_text(stmt, 9);
        if (seguidoresStr != nullptr && strlen(seguidoresStr) > 0) {
            std::stringstream ss(seguidoresStr);
            std::string token;
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) {
                    user.addFollower(token);
                }
            }
        }
        
        // Carregar following
        const char* seguindoStr = (const char*)sqlite3_column_text(stmt, 10);
        if (seguindoStr != nullptr && strlen(seguindoStr) > 0) {
            std::stringstream ss(seguindoStr);
            std::string token;
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) {
                    user.addFollowing(token);
                }
            }
        }
        
        usuarios.push_back(user);
    }
    
    sqlite3_finalize(stmt);
    std::cout << "DB Manager -> " << usuarios.size() << " usuários carregados!" << std::endl;
    return usuarios;
}

// ==================== COMPANIES ====================

bool DbManager::saveCompany(const std::vector<Company>& companies) {
    if (!verifyConnection()) return false;
    
    executerSQL("DELETE FROM company;");
    
    for (const auto& company : companies) {
        // company é const, então precisa de const_cast para chamar versão não-const
        const auto& seguidores = const_cast<Company&>(company).getFollowers();
        const auto& seguindo = const_cast<Company&>(company).getFollowing();
        std::string seguidoresStr = "";
        for (size_t i = 0; i < seguidores.size(); ++i) {
            seguidoresStr += seguidores[i];
            if (i < seguidores.size() - 1) seguidoresStr += ",";
        }
        
        std::string seguindoStr = "";
        for (size_t i = 0; i < seguindo.size(); ++i) {
            seguindoStr += seguindo[i];
            if (i < seguindo.size() - 1) seguindoStr += ",";
        }
     
        
        std::ostringstream sql;
        sql << "INSERT INTO company (id, nome, email, phone_number, password, state, descricao, cnpj, seguidores, seguindo) "
            << "VALUES (" << company.getId() << ", '"
            << company.getName() << "', '"
            << company.getEmail() << "', '"
            << company.getPhoneNumber() << "', '"
            << company.getPassword() << "', "
            << (company.getState() ? 1 : 0) << ", '"
            << company.getDescription() << "', '"
            << company.getCnpj() << "', '"
            << seguidoresStr << "', '"
            << seguindoStr << "');";
        
        if (!executerSQL(sql.str())) {
            std::cerr << "Erro ao salvar company: " << company.getName() << std::endl;
            return false;
        }
    }
    
    std::cout << "DB Manager -> " << "Companies salvas!" << std::endl;
    return true;
}


std::vector<Company> DbManager::loadCompanies() {
    std::vector<Company> companies;
    
    if (!verifyConnection()) return companies;
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM company;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
        return companies;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
    
        
        const char* nome = (const char*)sqlite3_column_text(stmt, 1);
        const char* email = (const char*)sqlite3_column_text(stmt, 2);
        const char* phone_number = (const char*)sqlite3_column_text(stmt, 3);
        const char* password = (const char*)sqlite3_column_text(stmt, 4);
        bool state = sqlite3_column_int(stmt, 5) != 0;
        const char* descricao = (const char*)sqlite3_column_text(stmt, 6);
        const char* cnpj = (const char*)sqlite3_column_text(stmt, 7);
        
        
        Company company(
            nome != nullptr ? nome : "",
            email != nullptr ? email : "",
            phone_number != nullptr ? phone_number : "31912345678",
            password != nullptr ? password : "",
            state,
            descricao != nullptr ? descricao : "",
            cnpj != nullptr ? cnpj : "12345678000199"
        );
        
        // Carregar followers
        const char* seguidoresStr = (const char*)sqlite3_column_text(stmt, 8);
        if (seguidoresStr != nullptr && strlen(seguidoresStr) > 0) {
            std::stringstream ss(seguidoresStr);
            std::string token;
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) {
                    company.addFollower(token);
                }
            }
        }
        
        // Carregar following
        const char* seguindoStr = (const char*)sqlite3_column_text(stmt, 9);
        if (seguindoStr != nullptr && strlen(seguindoStr) > 0) {
            std::stringstream ss(seguindoStr);
            std::string token;
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) {
                    company.addFollowing(token);
                }
            }
        }
        
        companies.push_back(company);
    }
    
    sqlite3_finalize(stmt);
    std::cout << "DB Manager -> " << companies.size() << " companies carregadas!" << std::endl;
    return companies;
}

// ==================== CASES GLOBAIS ====================

bool DbManager::saveCase(const CaseSystem& caseSystem) {
    if (!verifyConnection()) return false;
    
    executerSQL("DELETE FROM cases_globais;");
    
    const auto& cases = caseSystem.getAllCases();
    
    for (const auto& c : cases) {
        if (!c) continue;
        
        std::ostringstream sql;
        sql << "INSERT INTO cases_globais (id, nome, descricao, solucao, pontos, estado) "
            << "VALUES (" << c->getId() << ", '"
            << c->getName() << "', '"
            << c->getDescription() << "', '"
            << c->getSolution() << "', "
            << c->getPoints() << ", "
            << (c->getState() ? 1 : 0) << ");";
        
        if (!executerSQL(sql.str())) {
            std::cerr << "Erro ao salvar case global: " << c->getName() << std::endl;
            return false;
        }
    }
    
    std::cout << "DB Manager -> " << "Cases globais salvos!" << std::endl;
    return true;
}

CaseSystem DbManager::loadGlobalCases() {
    CaseSystem caseSystem;
    
    if (!verifyConnection()) return caseSystem;
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM cases_globais;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return caseSystem;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* nome = (const char*)sqlite3_column_text(stmt, 1);
        const char* descricao = (const char*)sqlite3_column_text(stmt, 2);
        const char* solucao = (const char*)sqlite3_column_text(stmt, 3);
        double pontos = sqlite3_column_double(stmt, 4);
        int estado = sqlite3_column_int(stmt, 5);
        
        auto newCase = std::make_shared<Case>(
            nome != nullptr ? nome : "", 
            descricao != nullptr ? descricao : "",
            solucao,
            pontos
        );

        newCase->setState(estado != 0);
        
        caseSystem.addCase(newCase);
    }
    
    sqlite3_finalize(stmt);
    std::cout << "DB Manager -> " << caseSystem.getAllCases().size() << " cases globais carregados!" << std::endl;
    return caseSystem;
}

// ==================== CASES POR COMPANY ====================

bool DbManager::saveCompanyCases(int companyId, const CaseSystem& caseSystem) {
    if (!verifyConnection()) return false;
    
    std::ostringstream sqlDelete;
    sqlDelete << "DELETE FROM cases_company WHERE company_id = " << companyId << ";";
    executerSQL(sqlDelete.str());
    
    const auto& cases = caseSystem.getAllCases();
    
    for (const auto& c : cases) {
        if (!c) continue;
        
        std::ostringstream sql;
        sql << "INSERT INTO cases_company (case_id, company_id, nome, descricao, solucao, pontos, estado) "
            << "VALUES (" << c->getId() << ", " << companyId << ", '"
            << c->getName() << "', '"
            << c->getDescription() << "', '"
            << c->getSolution() << "', "
            << c->getPoints() << ", "
            << (c->getState() ? 1 : 0) << ");";
        
        if (!executerSQL(sql.str())) {
            return false;
        }
    }
    
    std::cout << "DB Manager -> " << "Cases da company " << companyId << " salvos!" << std::endl;
    return true;
}

CaseSystem DbManager::loadCompanyCases(int companyId) {
    CaseSystem caseSystem;
    
    if (!verifyConnection()) return caseSystem;
    
    sqlite3_stmt* stmt;
    std::ostringstream sql;
    sql << "SELECT * FROM cases_company WHERE company_id = " << companyId << ";";
    
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return caseSystem;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* nome = (const char*)sqlite3_column_text(stmt, 3);
        const char* descricao = (const char*)sqlite3_column_text(stmt, 4);
        const char* solucao = (const char*)sqlite3_column_text(stmt, 5);
        double pontos = sqlite3_column_double(stmt, 6);
        int estado = sqlite3_column_int(stmt, 7);
        
        auto newCase = std::make_shared<Case>(
            nome != nullptr ? nome : "", 
            descricao != nullptr ? descricao : "",
            solucao,
            pontos
        );
        
        newCase->setState(estado != 0);
        
        caseSystem.addCase(newCase);
    }
    
    sqlite3_finalize(stmt);
    return caseSystem;
}

// ==================== RANKING GLOBAL ====================

bool DbManager::saveGlobalRanking(const std::vector<std::shared_ptr<User>>& users) {
    if (!verifyConnection()) return false;
    
    executerSQL("DELETE FROM ranking_global;");
    
    for (const auto& user : users) {
        if (!user) continue;
        
        std::ostringstream sql;
        sql << "INSERT INTO ranking_global (usuario_id, pontos) "
            << "VALUES (" << user->getId() << ", " << user->getTotalScore() << ");";
        
        if (!executerSQL(sql.str())) {
            return false;
        }
    }
    
    std::cout << "DB Manager -> " << "Ranking global salvo!" << std::endl;
    return true;
}

std::vector<int> DbManager::loadGlobalRanking() {
    std::vector<int> userIds;
    
    if (!verifyConnection()) return userIds;
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT usuario_id FROM ranking_global ORDER BY posicao;";
    
    if (sqlite3_prepare_v2(db , sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return userIds;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int userId = sqlite3_column_int(stmt, 0);
        userIds.push_back(userId);
    }
    
    sqlite3_finalize(stmt);
    std::cout << "DB Manager -> " << "Ranking global carregado!" << std::endl;
    return userIds;
}

// ==================== RANKING POR COMPANY ====================

bool DbManager::saveCompanyRanking(int companyId, const std::vector<std::shared_ptr<User>>& users) {
    if (!verifyConnection()) return false;
    
    std::ostringstream sqlDelete;
    sqlDelete << "DELETE FROM ranking_company WHERE company_id = " << companyId << ";";
    executerSQL(sqlDelete.str());
    
    int posicao = 1;
    for (const auto& user : users) {
        if (!user) continue;
        
        std::ostringstream sql;
        sql << "INSERT OR REPLACE INTO ranking_company (company_id, usuario_id, pontos, posicao) "
            << "VALUES (" << companyId << ", " << user->getId() << ", "
            << user->getTotalScore() << ", " << posicao << ");";
        
        if (!executerSQL(sql.str())) {
            return false;
        }
        posicao++;
    }
    
    std::cout << "DB Manager -> " << "Ranking da company " << companyId << " salvo!" << std::endl;
    return true;
}

std::vector<int> DbManager::loadCompanyRanking(int companyId) {
    std::vector<int> userIds;
    
    if (!verifyConnection()) return userIds;
    
    sqlite3_stmt* stmt;
    std::ostringstream sql;
    sql << "SELECT usuario_id FROM ranking_company WHERE company_id = " << companyId 
        << " ORDER BY posicao;";
    
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return userIds;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int userId = sqlite3_column_int(stmt, 0);
        userIds.push_back(userId);
    }
    
    sqlite3_finalize(stmt);
    return userIds;
}

// ==================== SOLVED CASES ====================

bool DbManager::saveSolvedCases(int userId, const std::vector<int>& solvedCaseIds) {
    if (!verifyConnection()) return false;
    
    std::ostringstream sqlDelete;
    sqlDelete << "DELETE FROM solved_cases WHERE usuario_id = " << userId << ";";
    executerSQL(sqlDelete.str());
    
    for (int caseId : solvedCaseIds) {
        std::ostringstream sql;
        sql << "INSERT INTO solved_cases (usuario_id, case_id) "
            << "VALUES (" << userId << ", " << caseId << ");";
        
        if (!executerSQL(sql.str())) {
            return false;
        }
    }
    
    return true;
}

std::vector<int> DbManager::loadSolvedCases(int userId) {
    std::vector<int> solvedCases;
    
    if (!verifyConnection()) return solvedCases;
    
    sqlite3_stmt* stmt;
    std::ostringstream sql;
    sql << "SELECT case_id FROM solved_cases WHERE usuario_id = " << userId << ";";
    
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return solvedCases;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int caseId = sqlite3_column_int(stmt, 0);
        solvedCases.push_back(caseId);
    }
    
    sqlite3_finalize(stmt);
    return solvedCases;
}

std::vector<std::shared_ptr<Post>> DbManager::loadPosts() {
    std::vector<std::shared_ptr<Post>> posts;
    
    if (!verifyConnection()) return posts;
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM posts;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Erro ao preparar SQL: " << sqlite3_errmsg(db) << std::endl;
        return posts;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* autorNome = (const char*)sqlite3_column_text(stmt, 2);
        const char* conteudo = (const char*)sqlite3_column_text(stmt, 3);
        int curtidas = sqlite3_column_int(stmt, 4);
        
        try {
            // 
            auto newPost = std::make_shared<Post>(
                autorNome != nullptr ? autorNome : "",
                conteudo != nullptr ? conteudo : ""
            );
            
            
            for (int i = 0; i < curtidas; ++i) {
                newPost->addLiker("");
            }
            
            posts.push_back(newPost);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao carregar post: " << e.what() << "\n";
            continue;
        }
    }
    
    sqlite3_finalize(stmt);
    std::cout << "DB Manager -> " << posts.size() << " posts carregados!\n";
    return posts;
}

bool DbManager::savePosts(const std::vector<std::shared_ptr<Post>>& posts) {
    if (!verifyConnection()) return false;
    
    executerSQL("DELETE FROM posts;");
    
    for (const auto& post : posts) {
        if (!post) continue;
        
        try {
            std::ostringstream sql;
            sql << "INSERT INTO posts (usuario_id, autor_nome, conteudo, curtidas) "
                << "VALUES (0, '"  
                << post->getAuthor() << "', '"  // 
                << post->getDescription() << "', "
                << post->getNumLikes() << ")";

            if (!executerSQL(sql.str())) {
                std::cerr << "Erro ao salvar post!\n";
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro ao salvar post: " << e.what() << "\n";
            return false;
        }
    }
    
    std::cout << "DB Manager -> " << "Posts salvos!\n";
    return true;
}
