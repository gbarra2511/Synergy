// Synergy - Authentication Module

// Toggle password visibility
function togglePassword() {
    const passwordInput = document.getElementById('password');
    const isPassword = passwordInput.type === 'password';
    passwordInput.type = isPassword ? 'text' : 'password';
}

// Handle login
function handleLogin(event) {
    event.preventDefault();

    const email = document.getElementById('email').value;
    const password = document.getElementById('password').value;

    // TODO: Integrar com backend
    // Por enquanto, simular login
    console.log('Login attempt:', { email, password });

    // Simular delay de rede
    showLoading();

    setTimeout(() => {
        // Salvar dados do usuário no localStorage
        const userData = {
            id: 1,
            name: 'Usuário Demo',
            email: email,
            type: 'user',
            avatar: null,
            totalScore: 1250,
            solvedCases: 15,
            ranking: 42,
            connections: 127
        };

        localStorage.setItem('currentUser', JSON.stringify(userData));
        localStorage.setItem('isAuthenticated', 'true');

        hideLoading();

        // Redirecionar para dashboard
        window.location.href = 'dashboard.html';
    }, 1000);
}

// Handle register
function handleRegister(event) {
    event.preventDefault();

    const form = event.target;
    const formData = new FormData(form);

    // TODO: Integrar com backend
    console.log('Register attempt:', Object.fromEntries(formData));

    showLoading();

    setTimeout(() => {
        hideLoading();
        closeRegisterModal();

        // Mostrar mensagem de sucesso
        showAlert('Conta criada com sucesso! Faça login para continuar.', 'success');
    }, 1000);
}

// Show/hide register modal
function showRegisterModal() {
    const modal = document.getElementById('registerModal');
    modal.classList.add('active');
}

function closeRegisterModal() {
    const modal = document.getElementById('registerModal');
    modal.classList.remove('active');
}

// Switch between user and company tabs
let currentAccountType = 'user';

function switchTab(type) {
    currentAccountType = type;

    // Update tab active state
    document.querySelectorAll('.tab').forEach(tab => {
        tab.classList.remove('active');
    });
    event.target.classList.add('active');

    // Show/hide fields
    const userFields = document.getElementById('userFields');
    const companyFields = document.getElementById('companyFields');

    if (type === 'user') {
        userFields.classList.remove('d-none');
        companyFields.classList.add('d-none');

        // Make user fields required
        userFields.querySelectorAll('input').forEach(input => {
            input.required = true;
        });
        companyFields.querySelectorAll('input').forEach(input => {
            input.required = false;
        });
    } else {
        userFields.classList.add('d-none');
        companyFields.classList.remove('d-none');

        // Make company fields required
        userFields.querySelectorAll('input').forEach(input => {
            input.required = false;
        });
        companyFields.querySelectorAll('input').forEach(input => {
            input.required = true;
        });
    }
}

// Loading state
function showLoading() {
    const submitBtn = document.querySelector('#loginForm button[type="submit"]');
    if (submitBtn) {
        submitBtn.disabled = true;
        submitBtn.innerHTML = '<div class="spinner" style="width: 20px; height: 20px; border-width: 2px;"></div>';
    }
}

function hideLoading() {
    const submitBtn = document.querySelector('#loginForm button[type="submit"]');
    if (submitBtn) {
        submitBtn.disabled = false;
        submitBtn.innerHTML = 'Entrar';
    }
}

// Show alert
function showAlert(message, type = 'info') {
    const alertDiv = document.createElement('div');
    alertDiv.className = `alert alert-${type}`;
    alertDiv.textContent = message;

    const container = document.querySelector('.login-form-container');
    container.insertBefore(alertDiv, container.firstChild);

    setTimeout(() => {
        alertDiv.remove();
    }, 5000);
}

// Close modal on overlay click
document.addEventListener('click', (e) => {
    if (e.target.classList.contains('modal-overlay')) {
        closeRegisterModal();
    }
});

// Check if already logged in
window.addEventListener('DOMContentLoaded', () => {
    const isAuthenticated = localStorage.getItem('isAuthenticated');
    if (isAuthenticated === 'true' && window.location.pathname.includes('index.html')) {
        window.location.href = 'dashboard.html';
    }
});
