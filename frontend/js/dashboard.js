// Synergy - Dashboard Module

// State Management
let currentUser = null;
let currentView = 'feed';
let currentCaseId = null;

// Initialize Dashboard
document.addEventListener('DOMContentLoaded', () => {
    // Check authentication
    const isAuthenticated = localStorage.getItem('isAuthenticated');
    if (isAuthenticated !== 'true') {
        window.location.href = 'index.html';
        return;
    }

    // Load user data
    currentUser = JSON.parse(localStorage.getItem('currentUser'));
    if (currentUser) {
        updateUserInterface();
    }

    // Load initial data
    loadFeedPosts();
    loadCases();
    loadRanking();
    loadSuggestedUsers();
});

// Update User Interface
function updateUserInterface() {
    if (!currentUser) return;

    document.getElementById('userName').textContent = currentUser.name;
    document.getElementById('profileName').textContent = currentUser.name;
    document.getElementById('profileEmail').textContent = currentUser.email;
    document.getElementById('profileScore').textContent = currentUser.totalScore;
    document.getElementById('profileCases').textContent = currentUser.solvedCases;
    document.getElementById('profileRanking').textContent = `#${currentUser.ranking}`;
    document.getElementById('profileConnections').textContent = currentUser.connections;

    // Update avatar initials
    const initial = currentUser.name.charAt(0).toUpperCase();
    document.querySelectorAll('.avatar span').forEach(avatar => {
        avatar.textContent = initial;
    });
}

// Navigation
function navigateTo(view) {
    currentView = view;

    // Update nav items
    document.querySelectorAll('.nav-item').forEach(item => {
        item.classList.remove('active');
    });
    event.target.closest('.nav-item').classList.add('active');

    // Hide all views
    document.querySelectorAll('.content-view').forEach(view => {
        view.classList.remove('active');
    });

    // Show selected view
    const viewElement = document.getElementById(`${view}View`);
    if (viewElement) {
        viewElement.classList.add('active');
    }

    // Close mobile menu if open
    closeMobileMenu();
}

// Mobile Menu
function toggleMobileMenu() {
    const sidebar = document.getElementById('sidebar');
    sidebar.classList.toggle('mobile-open');
}

function closeMobileMenu() {
    const sidebar = document.getElementById('sidebar');
    sidebar.classList.remove('mobile-open');
}

// Feed Functions
function loadFeedPosts() {
    const postsContainer = document.getElementById('postsContainer');

    // Mock data - replace with actual API call
    const posts = [
        {
            id: 1,
            author: { name: 'Maria Silva', avatar: 'M', title: 'Desenvolvedora Full Stack' },
            content: 'Acabei de resolver um case incrível sobre otimização de banco de dados! A sensação de conseguir melhorar a performance em 40% é indescritível. 🚀',
            timestamp: '2 horas atrás',
            likes: 24,
            comments: 5,
            liked: false
        },
        {
            id: 2,
            author: { name: 'João Santos', avatar: 'J', title: 'Data Scientist' },
            content: 'Participando do case da TechCorp sobre análise preditiva. Alguém mais está trabalhando nisso? Seria legal trocar ideias!',
            timestamp: '5 horas atrás',
            likes: 12,
            comments: 8,
            liked: false
        },
        {
            id: 3,
            author: { name: 'Ana Costa', avatar: 'A', title: 'UX Designer' },
            content: 'Novo marco alcançado! 🎉 Cheguei aos 1000 pontos no ranking. Obrigada a todos que me apoiaram nessa jornada. A próxima meta é o top 10!',
            timestamp: '1 dia atrás',
            likes: 45,
            comments: 12,
            liked: true
        }
    ];

    postsContainer.innerHTML = posts.map(post => `
        <div class="post-card">
            <div class="post-header">
                <div class="avatar">${post.author.avatar}</div>
                <div class="post-author-info">
                    <div class="post-author-name">${post.author.name}</div>
                    <div class="post-time">${post.author.title} • ${post.timestamp}</div>
                </div>
                <button class="btn-icon">⋮</button>
            </div>

            <div class="post-content">${post.content}</div>

            <div class="post-actions">
                <button class="post-action-btn ${post.liked ? 'liked' : ''}" onclick="toggleLike(${post.id})">
                    <span>${post.liked ? '❤️' : '🤍'}</span>
                    <span>${post.likes}</span>
                </button>
                <button class="post-action-btn" onclick="openComments(${post.id})">
                    <span>💬</span>
                    <span>${post.comments}</span>
                </button>
                <button class="post-action-btn">
                    <span>🔄</span>
                    <span>Compartilhar</span>
                </button>
            </div>
        </div>
    `).join('');
}

function openCreatePostModal() {
    document.getElementById('createPostModal').classList.add('active');
}

function closeCreatePostModal() {
    document.getElementById('createPostModal').classList.remove('active');
    document.getElementById('postContent').value = '';
}

function handleCreatePost(event) {
    event.preventDefault();

    const content = document.getElementById('postContent').value;

    // TODO: Send to backend
    console.log('Creating post:', content);

    // Simulate API call
    setTimeout(() => {
        closeCreatePostModal();
        showNotification('Post publicado com sucesso!', 'success');
        loadFeedPosts(); // Reload feed
    }, 500);
}

function toggleLike(postId) {
    // TODO: Send to backend
    console.log('Toggle like for post:', postId);

    // Update UI
    loadFeedPosts();
}

function openComments(postId) {
    console.log('Open comments for post:', postId);
    // TODO: Implement comments modal
}

// Cases Functions
function loadCases() {
    const casesContainer = document.getElementById('casesContainer');

    // Mock data - replace with actual API call
    const cases = [
        {
            id: 1,
            title: 'Otimização de Algoritmo de Busca',
            company: 'TechCorp',
            description: 'Desenvolva um algoritmo eficiente para buscar elementos em uma estrutura de dados complexa, considerando performance e uso de memória.',
            points: 150,
            difficulty: 'Médio',
            tags: ['Algoritmos', 'Performance', 'Estrutura de Dados']
        },
        {
            id: 2,
            title: 'Análise de Dados de Vendas',
            company: 'DataCo',
            description: 'Analise um conjunto de dados de vendas e identifique padrões, tendências e oportunidades de crescimento.',
            points: 200,
            difficulty: 'Difícil',
            tags: ['Data Science', 'Python', 'Análise']
        },
        {
            id: 3,
            title: 'Design de Interface Responsiva',
            company: 'DesignHub',
            description: 'Crie uma interface responsiva para um aplicativo de gerenciamento de tarefas, focando em UX e acessibilidade.',
            points: 120,
            difficulty: 'Fácil',
            tags: ['UI/UX', 'Design', 'Responsivo']
        },
        {
            id: 4,
            title: 'API REST para E-commerce',
            company: 'ShopTech',
            description: 'Desenvolva uma API REST completa para um sistema de e-commerce, incluindo autenticação, produtos e carrinho.',
            points: 180,
            difficulty: 'Médio',
            tags: ['Backend', 'API', 'Node.js']
        },
        {
            id: 5,
            title: 'Machine Learning para Recomendações',
            company: 'AI Solutions',
            description: 'Implemente um sistema de recomendação usando Machine Learning para sugerir produtos baseado no histórico do usuário.',
            points: 250,
            difficulty: 'Difícil',
            tags: ['ML', 'Python', 'Recomendação']
        },
        {
            id: 6,
            title: 'Gestão de Projeto Ágil',
            company: 'AgileCo',
            description: 'Proponha uma estratégia de gestão ágil para um projeto de desenvolvimento de software com equipe distribuída.',
            points: 100,
            difficulty: 'Fácil',
            tags: ['Gestão', 'Agile', 'Scrum']
        }
    ];

    casesContainer.innerHTML = cases.map(caseItem => `
        <div class="case-card" onclick="openCaseDetail(${caseItem.id})">
            <div class="case-header">
                <div>
                    <div class="case-title">${caseItem.title}</div>
                    <div class="case-company">${caseItem.company}</div>
                </div>
                <div class="case-points">${caseItem.points} pts</div>
            </div>

            <div class="case-description">${caseItem.description}</div>

            <div class="case-tags">
                <span class="badge badge-info">${caseItem.difficulty}</span>
                ${caseItem.tags.map(tag => `<span class="case-tag">${tag}</span>`).join('')}
            </div>
        </div>
    `).join('');
}

function openCaseDetail(caseId) {
    currentCaseId = caseId;

    // Mock data - replace with actual API call
    const caseData = {
        id: caseId,
        title: 'Otimização de Algoritmo de Busca',
        company: 'TechCorp',
        description: 'Desenvolva um algoritmo eficiente para buscar elementos em uma estrutura de dados complexa.',
        detailedDescription: `
            <h3 class="mb-md">Descrição Detalhada</h3>
            <p class="mb-md">Você precisa implementar um algoritmo de busca que seja eficiente tanto em termos de tempo quanto de espaço. O algoritmo deve ser capaz de:</p>
            <ul class="mb-md" style="margin-left: 20px;">
                <li>Buscar elementos em O(log n) ou melhor</li>
                <li>Utilizar no máximo O(n) de memória</li>
                <li>Lidar com dados dinâmicos (inserções e remoções)</li>
            </ul>
            <h3 class="mb-md">Critérios de Avaliação</h3>
            <ul style="margin-left: 20px;">
                <li>Complexidade temporal e espacial</li>
                <li>Qualidade do código</li>
                <li>Tratamento de casos extremos</li>
                <li>Documentação</li>
            </ul>
        `,
        points: 150
    };

    document.getElementById('caseDetailTitle').textContent = caseData.title;
    document.getElementById('caseDetailContent').innerHTML = caseData.detailedDescription;

    document.getElementById('caseDetailModal').classList.add('active');
}

function closeCaseDetailModal() {
    document.getElementById('caseDetailModal').classList.remove('active');
    document.getElementById('caseSolution').value = '';
    currentCaseId = null;
}

function handleSubmitSolution(event) {
    event.preventDefault();

    const solution = document.getElementById('caseSolution').value;

    // TODO: Send to backend
    console.log('Submitting solution for case:', currentCaseId, solution);

    // Simulate API call
    setTimeout(() => {
        closeCaseDetailModal();
        showNotification('Solução enviada com sucesso! Aguarde a avaliação.', 'success');
    }, 500);
}

// Ranking Functions
function loadRanking() {
    const rankingContainer = document.getElementById('rankingContainer');

    // Mock data - replace with actual API call
    const ranking = [
        { position: 1, name: 'Carlos Mendes', avatar: 'C', cases: 45, score: 3250 },
        { position: 2, name: 'Julia Alves', avatar: 'J', cases: 42, score: 3100 },
        { position: 3, name: 'Pedro Lima', avatar: 'P', cases: 40, score: 2980 },
        { position: 4, name: 'Fernanda Costa', avatar: 'F', cases: 38, score: 2850 },
        { position: 5, name: 'Ricardo Souza', avatar: 'R', cases: 35, score: 2700 },
        { position: 42, name: currentUser?.name || 'Você', avatar: 'U', cases: currentUser?.solvedCases || 0, score: currentUser?.totalScore || 0, highlight: true }
    ];

    rankingContainer.innerHTML = ranking.map(user => `
        <div class="ranking-row ${user.highlight ? 'highlight' : ''}">
            <div class="rank-position">${user.position}</div>
            <div class="rank-user">
                <div class="avatar avatar-sm">${user.avatar}</div>
                <span class="font-semibold">${user.name}</span>
            </div>
            <div class="text-center cases-solved">${user.cases}</div>
            <div class="text-center font-bold">${user.score}</div>
        </div>
    `).join('');
}

// Network Functions
let currentNetworkTab = 'following';

function switchNetworkTab(tab) {
    currentNetworkTab = tab;

    // Update tab active state
    document.querySelectorAll('#networkView .tab').forEach(tabEl => {
        tabEl.classList.remove('active');
    });
    event.target.classList.add('active');

    loadNetworkConnections();
}

function loadNetworkConnections() {
    const networkContainer = document.getElementById('networkContainer');

    // Mock data - replace with actual API call
    const connections = [
        { id: 1, name: 'Maria Silva', title: 'Desenvolvedora Full Stack', avatar: 'M', following: true },
        { id: 2, name: 'João Santos', title: 'Data Scientist', avatar: 'J', following: false },
        { id: 3, name: 'Ana Costa', title: 'UX Designer', avatar: 'A', following: true },
        { id: 4, name: 'Paulo Oliveira', title: 'Product Manager', avatar: 'P', following: false }
    ];

    networkContainer.innerHTML = connections.map(user => `
        <div class="card">
            <div class="d-flex align-center gap-md mb-md">
                <div class="avatar avatar-lg">${user.avatar}</div>
                <div class="flex-1">
                    <div class="font-bold">${user.name}</div>
                    <div class="text-secondary text-sm">${user.title}</div>
                </div>
            </div>
            <button class="btn ${user.following ? 'btn-secondary' : 'btn-primary'} w-full" onclick="toggleFollow(${user.id})">
                ${user.following ? 'Deixar de Seguir' : 'Seguir'}
            </button>
        </div>
    `).join('');
}

function toggleFollow(userId) {
    // TODO: Send to backend
    console.log('Toggle follow for user:', userId);

    showNotification('Conexão atualizada!', 'success');
    loadNetworkConnections();
}

// Suggested Users
function loadSuggestedUsers() {
    const suggestedContainer = document.getElementById('suggestedUsers');

    const suggestions = [
        { id: 1, name: 'Laura Santos', title: 'Tech Lead', avatar: 'L' },
        { id: 2, name: 'Bruno Costa', title: 'DevOps Engineer', avatar: 'B' },
        { id: 3, name: 'Carla Lima', title: 'Scrum Master', avatar: 'C' }
    ];

    suggestedContainer.innerHTML = suggestions.map(user => `
        <div class="suggested-user">
            <div class="avatar avatar-sm">${user.avatar}</div>
            <div class="suggested-user-info">
                <div class="suggested-user-name">${user.name}</div>
                <div class="suggested-user-title">${user.title}</div>
            </div>
            <button class="follow-btn" onclick="followUser(${user.id})">Seguir</button>
        </div>
    `).join('');
}

function followUser(userId) {
    // TODO: Send to backend
    console.log('Follow user:', userId);

    showNotification('Você começou a seguir este usuário!', 'success');
    loadSuggestedUsers();
}

// Utility Functions
function showNotification(message, type = 'info') {
    // Create notification element
    const notification = document.createElement('div');
    notification.className = `alert alert-${type}`;
    notification.style.position = 'fixed';
    notification.style.top = '20px';
    notification.style.right = '20px';
    notification.style.zIndex = '9999';
    notification.style.minWidth = '300px';
    notification.textContent = message;

    document.body.appendChild(notification);

    // Remove after 3 seconds
    setTimeout(() => {
        notification.style.opacity = '0';
        notification.style.transition = 'opacity 0.3s ease';

        setTimeout(() => {
            notification.remove();
        }, 300);
    }, 3000);
}

function logout() {
    localStorage.removeItem('currentUser');
    localStorage.removeItem('isAuthenticated');
    window.location.href = 'index.html';
}

// Close modals on overlay click
document.addEventListener('click', (e) => {
    if (e.target.classList.contains('modal-overlay')) {
        closeCreatePostModal();
        closeCaseDetailModal();
    }
});

// Close mobile menu when clicking outside
document.addEventListener('click', (e) => {
    const sidebar = document.getElementById('sidebar');
    const menuBtn = document.querySelector('.mobile-menu-btn');

    if (!sidebar.contains(e.target) && !menuBtn.contains(e.target)) {
        closeMobileMenu();
    }
});

// Filter cases
document.querySelectorAll('#casesView .filter-btn').forEach(btn => {
    btn.addEventListener('click', function() {
        document.querySelectorAll('#casesView .filter-btn').forEach(b => {
            b.classList.remove('active');
        });
        this.classList.add('active');

        // TODO: Filter cases based on category
        console.log('Filter by:', this.textContent);
    });
});
