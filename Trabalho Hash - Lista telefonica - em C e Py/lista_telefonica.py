import re
import time

class TabelaHash:
    def __init__(self, tamanho=10):
        """Inicializa a tabela hash com buckets vazios."""
        self.tamanho = tamanho
        self.tabela = [[] for _ in range(tamanho)]
    
    def funcao_hash(self, chave):
        """Calcula o índice usando a função hash."""
        return hash(chave) % self.tamanho
    
    def formatar_telefone(self, telefone):
        """Valida e formata o telefone para o padrão brasileiro."""
        telefone = re.sub(r'\D', '', telefone)
        
        if len(telefone) == 11:
            return f"({telefone[:2]}) {telefone[2:7]}-{telefone[7:]}"
        elif len(telefone) == 10:
            return f"({telefone[:2]}) {telefone[2:6]}-{telefone[6:]}"
        else:
            raise ValueError("Número de telefone inválido. Deve conter 10 ou 11 dígitos.")
    
    def inserir(self, nome, telefone):
        """Insere um nome e telefone na tabela."""
        try:
            telefone_formatado = self.formatar_telefone(telefone)
        except ValueError as e:
            print(f"Erro ao inserir {nome}: {e}")
            return
        
        indice = self.funcao_hash(nome)
        # Nome já existe?
        for par in self.tabela[indice]:
            if par[0] == nome:
                par[1] = telefone_formatado  # Atualiza o telefone
                print(f"{nome} já existia e foi atualizado.")
                return

        self.tabela[indice].append([nome, telefone_formatado])
        print(f"Contato {nome} adicionado com sucesso.")
    
    def buscar(self, nome):
        """Busca o telefone associado a um nome."""
        indice = self.funcao_hash(nome)
        for par in self.tabela[indice]:
            if par[0] == nome:
                return par[1]
        return None
    
    def remover(self, nome):
        """Remove um nome e telefone da tabela."""
        indice = self.funcao_hash(nome)
        for par in self.tabela[indice]:
            if par[0] == nome:
                self.tabela[indice].remove(par)
                print(f"Contato {nome} removido com sucesso.")
                return True
        print(f"Contato {nome} não encontrado.")
        return False
    
    def exibir(self):
        """Exibe todos os contatos na tabela hash."""
        print("Contatos na Lista Telefônica:")
        for bucket in self.tabela:
            for nome, telefone in bucket:
                print(f"{nome}: {telefone}")


def medir_tempo(func, *args, **kwargs):
    """Mede o tempo de execução de uma função."""
    inicio = time.time()
    resultado = func(*args, **kwargs)
    fim = time.time()
    return resultado, (fim - inicio) * 1000  # Tempo em milissegundos


def menu():
    """Interface de console para a lista telefônica."""
    agenda = TabelaHash()
    
    while True:
        print("\n--- Lista Telefônica ---")
        print("1 - Adicionar contato")
        print("2 - Buscar contato por nome")
        print("3 - Remover contato")
        print("4 - Exibir todos os contatos")
        print("0 - Sair")
        
        opcao = input("Digite uma opção: ")
        
        if opcao == "1":
            nome = input("Nome: ").strip()
            telefone = input("Telefone: ").strip()
            _, tempo = medir_tempo(agenda.inserir, nome, telefone)
            print(f"Operação concluída em {tempo:.4f} ms")
        
        elif opcao == "2":
            nome = input("Nome: ").strip()
            telefone, tempo = medir_tempo(agenda.buscar, nome)
            if telefone:
                print(f"Telefone de {nome}: {telefone} (tempo de busca: {tempo:.4f} ms)")
            else:
                print(f"Contato {nome} não encontrado. (tempo de busca: {tempo:.4f} ms)")
        
        elif opcao == "3":
            nome = input("Nome: ").strip()
            _, tempo = medir_tempo(agenda.remover, nome)
            print(f"Operação concluída em {tempo:.4f} ms")
        
        elif opcao == "4":
            _, tempo = medir_tempo(agenda.exibir)
            print(f"Operação concluída em {tempo:.4f} ms")
        
        elif opcao == "0":
            print("Encerrando o programa. Até logo!")
            break
        
        else:
            print("Opção inválida! Tente novamente.")

menu()
