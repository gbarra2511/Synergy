// #include "doctest.h"
// #include "Ranking.hpp"
// #include "User.hpp"

// TEST_CASE("1. Testando o Construtor de Ranking, GetUsers() e AddUser()") {
//     std::string nome1 = "Nome 1";
//     std::string email1 = "Email 1";
//     std::string cpf1 = "12345678900";
//     auto user1 = std::make_shared<User>(nome1, email1, cpf1);

//     std::string nome2 = "Nome 2";
//     std::string email2 = "Email 2";
//     std::string cpf2 = "12345678901";
//     auto user2 = std::make_shared<User>(nome2, email2, cpf2);

//     Ranking r;
//     r.addUser(user1);
//     r.addUser(user2);
//     CHECK(r.getUsers()[0]->getId() == user1->getId());
//     CHECK(r.getUsers()[1]->getId() == user2->getId());
// }
// TEST_CASE("2. Testando Ranking::sort()") {
//     auto user1 = std::make_shared<User>("Ana", "ana@email.com", "123.123.123-12");
//     auto user2 = std::make_shared<User>("Bruno", "bruno@email.com", "456.456.456-45");
//     auto user3 = std::make_shared<User>("Carla", "carla@email.com", "789.789.789-78");
    
//     user1->setTotalScore(20);
//     user2->setTotalScore(10);
//     user3->setTotalScore(30);

//     CHECK(user1->getTotalScore() == 20);
//     CHECK(user2->getTotalScore() == 10);
//     CHECK(user3->getTotalScore() == 30);


//     Ranking ranking;
//     ranking.addUser(user1);
//     ranking.addUser(user2);
//     ranking.addUser(user3);

//     ranking.sort();

//     auto sorted = ranking.getUsers();
//     CHECK(sorted[0]->getId() == user2->getId());
//     CHECK(sorted[1]->getId() == user1->getId());
//     CHECK(sorted[2]->getId() == user3->getId());
// }
