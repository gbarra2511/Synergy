## User Stories

# User Story 1 - Sistema de Criação de Conta Comum (Pessoa Física)

### Descrição
Como **Usuário Comum**, quero me cadastrar na plataforma para utilizar as funcionalidades.

### Critérios de Aceitação
- O **Sistema** deve solicitar no cadastro um **nome** e um **email** e/ou **telefone** ao usuário
- O **Usuário** deve preencher um nome, email e/ou telefone **único**
- O **Usuário** deve criar uma **senha válida** e compatível com os requisitos de segurança
- O **Sistema** deve validar o email e/ou telefone, gerando um **id único** caso o cadastro seja bem sucedido
- O **Usuário** apenas deve conseguir efetuar um **login** após a confirmação e validação de todos os passos supracitados


---


# User Story 2 - Sistema de Criação de Conta Empresarial

### Descrição
Como uma **Empresa**, quero cadastrar minha organização a fim de gerenciar os cases, propostas e identificar usuários promissores.

### Critérios de Aceitação
- O **Sistema** deve solicitar no cadastro informações empresariais como: **Razão Social**, **Nome**, **CNPJ**, **Endereço** e dados de login do representante
- A **Empresa** deve preencher as informações requeridas corretamente
- O **Sistema** deve validar as informações enviadas em busca de garantir a **unicidade** de cada empresa no sistema
- O **Usuário** deve disponibilizar cases de todos os **níveis de dificuldade**
- O **Sistema** deve sugerir aos usuários comuns quais cases resolver

---


# User Story 3 - Sistema de Rankeamento dos Usuários

### Descrição
Como **usuário de uma conta comum**, quero responder Cases, além de disponibilizar meu currículo e possíveis conquistas que adquiri na plataforma para obter visibilidade maior diante as empresas.

### Critérios de Aceitação
- O **Usuário Candidato** deve responder o **Case** corretamente para obter **pontuação máxima**
- As **Empresas** cadastradas na plataforma devem ter fácil acesso à visualização da **pontuação** e do **perfil** do usuário
- O **Sistema** deve limitar o **número de tentativas** por intervalo de tempo para não sobrecarregar o próprio
- O **Sistema** deve ofertar ao Usuário **"Conquistas"** ou **"Badges"** de acordo como seu progresso na resolução de cases
- O **Usuário** deve poder exibir as **"Conquistas"** ou **"Badges"** em seu perfil

---

# User Story 4 - Sistema de Criação de Case

### Descrição
Como **Recrutador em uma Conta Empresarial**, quero criar e publicar novos cases na plataforma, informando título, marcadores e outras informações, a fim de avaliar as habilidades práticas dos candidatos.

### Critérios de Aceitação
- O **Recrutador** deve estar logado em uma **conta Empresarial válida** para ter acesso a funcionalidade de **"Criar Case"**
- O **Sistema** deve exigir e validar um **formulário de criação de case**, contendo, como exemplo, os seguintes dados:
  - **Título do Case**
  - **Descrição Completa do Desafio**
  - **Habilidades Técnicas Desejadas** para tal Desafios (Sistema de Tags)
  - **Cronometragem** submissão das respostas
  - **Informações adicionais**, se necessário (Conjunto de ferramenta etc...)
- O **Recrutador** deve poder **"Salvar como Rascunho"** para continuar a edição num momento posterior, sem que haja a submissão pública do Case
- Após a validação e publicação do desafio, o **case** deve torna-se **visível** para todos os usuários que:
  - Seguem o perfil da **Empresa** na plataforma
  - Adicionaram a(s) **Tag(s)** referenciadas no Case
  - São sinalizadas como possíveis interessados pelo **algoritmo** da plataforma
- O **Sistema** não deve permitir que a **"Data Limite"** seja anterior à **"Data de início"**

---

# User Story 5 - Sistema de Respostas dos Cases

### Descrição
Como um **candidato**,quero submeter minha resolução de um Case que esteja aberto, enviando a resolução e obter pontuação.

### Critérios de Aceitação
- O **candidato** deve estar logado em uma **conta comum** para submeter uma resolução
- Na página de detalhes de um **case**, estando ela dentro do **prazo de submissão**, deve aparecer ao candidato um botão como **"Responder ao Case"** ou **"Enviar Solução"**
- O **Sistema** deve disponibilizar ao Candidato durante a submissão do case um **campo textual** para a descrição da solução
- O **Sistema** deve emitir ao Candidato uma **mensagem de confirmação** na tela após o envio bem sucedido, além de bloquear a visualização e/ou desabilitar o botão **"Responder ao Case"** para o candidato
- O **Candidato** deve poder visualizar as **submissões bem sucedidas** em uma área pessoal do seu perfil, tal como deixá-las **públicas** para as empresas

---

# User Story 6 - Sistema de Interação entre Usuários (Empresa/Candidato)

### Descrição
Eu como **empresa**, quero buscar informações sobre os candidatos que participam da resolução do case proposto, a fim de verificar competências e escolher opções que mais se encaixem com o perfil procurado.

### Critérios de Aceitação
- A **Empresa** deve conseguir ter acesso aos **candidatos participantes** para poder avaliá-los
- O **Sistema** deve listar as **competências** fornecidas pelos usuários
- A **Empresa** deve conseguir selecionar as **solicitações de participação** no case proposto
- O **Candidato** deve conseguir entrar em **contato** com a empresa para sanar possíveis dúvidas sobre o case e/ou cargo oferecido
- A **empresa** deve ter acesso ao **contato** fornecido pelo candidato para se comunicar com ele durante o processo


---
