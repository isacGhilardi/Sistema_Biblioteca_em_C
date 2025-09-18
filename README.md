# 📚 Sistema de Gerenciamento de Biblioteca em C

Projeto desenvolvido como parte do **Trabalho Discente Efetivo (TDE)** da disciplina de **Estrutura de Dados** – 2º Período de ADS.

## 🚀 Objetivo
Implementar um sistema simples de gerenciamento de biblioteca utilizando **estruturas lineares em C**, aplicando conceitos de **inserção, remoção e busca**.

## 🛠️ Estruturas Utilizadas
- **Vetores (arrays):** usados para strings (título do livro, nome do leitor).
- **Lista Encadeada Simples:** cadastro de **livros** e **leitores**.
- **Lista Duplamente Encadeada:** controle de **empréstimos**.
- **Pilha:** histórico de operações (cadastros, empréstimos e devoluções).

## 📌 Funcionalidades
- Cadastrar livros 📖
- Cadastrar leitores 👤
- Listar livros disponíveis e emprestados
- Realizar empréstimos 🔄
- Realizar devoluções ✅
- Visualizar histórico de operações 📜
- Desfazer última operação ↩️

## 📂 Estrutura do Código
- `struct livro` → lista simples de livros  
- `struct leitor` → lista simples de leitores  
- `struct emprestimo` → lista dupla de empréstimos  
- `struct operacao` → pilha de histórico  

## 🖥️ Execução
Compile com:
```bash
gcc biblioteca.c -o biblioteca
./biblioteca
