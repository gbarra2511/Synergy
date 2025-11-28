# Instruções gerais de compilação

- **C++11** ou superior (C++17 é util)
- **SQLite3**
- **Make** 
- **Compilador G++** 


---##  Compilação do Projeto

### **Passo 1: Clonar o Repositório**

```bash
git clone https://github.com/pds2-dcc-ufmg/2025-2-TW-TE-grupo09.git
cd synergy
```

---

### **Passo 2: Limpar Compilações Anteriores**

```bash
make clean
```

---

### **Passo 3: Compilar o Projeto**

```bash
make
```

**O que acontece**:
-  Compila todos os arquivos `.cpp` em arquivos objeto `.o` na pasta `build/`
-  Linka tudo em um executável em `bin/synergy`
-  Cria a pasta `bin/` automaticamente se não existir

### **Passo 4: Executar o Programa**

```bash
./bin/synergy
```
ou no Windows CMD:
```bash
bin\synergy.exe
```

ou no Windows CMD:
```bash
bin\synergy.exe
```

---
