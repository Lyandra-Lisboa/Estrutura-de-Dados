class SistemaDeVotacao:
    def __init__(self, tamanho=10):
        """Inicializa a tabela hash para o sistema de votação."""
        self.tamanho = tamanho
        self.tabela = [[] for _ in range(tamanho)]
        self.votos = {}

    def funcao_hash(self, chave):
        """Calcula o índice usando a função hash."""
        return hash(chave) % self.tamanho

    def buscar_usuario(self, id):
        """Busca se um usuário consta na tabela."""
        indice = self.funcao_hash(id)
        for item in self.tabela[indice]:
            if item[0] == id:
                return item
        return None

    def votar(self, id, candidato):
        """Registra o voto do eleitor, verificando se ele já votou."""
        # Eleitor já votou?
        if self.buscar_usuario(id):
            print("Erro: Eleitor já votou.")
            return

        # Registra o voto para o candidato
        if candidato in self.votos:
            self.votos[candidato] += 1
        else:
            self.votos[candidato] = 1

        # Adiciona usuário na tabela hash
        indice = self.funcao_hash(id)
        self.tabela[indice].append([id, candidato])
        print(f"Voto registrado para {candidato}!")

    def obter_resultado(self):
        """Exibe o resultado da votação."""
        if not self.votos:
            return "Nenhum voto registrado ainda."
        resultado = "Resultados da eleição:\n"
        for candidato, votos in self.votos.items():
            resultado += f"{candidato}: {votos} voto(s)\n"
        return resultado.strip()


def menu():
    """Interface de console para a votação."""
    sistema = SistemaDeVotacao()

    while True:
        print("\n--- Sistema de Votação ---")
        print("1 - Registrar voto")
        print("2 - Mostrar resultados")
        print("0 - Sair")

        opcao = input("Digite uma opção: ").strip()

        if opcao == "1":
            id_eleitor = input("Digite o ID do eleitor: ").strip()
            candidato = input("Digite o nome do candidato: ").strip()
            sistema.votar(id_eleitor, candidato)
        elif opcao == "2":
            print(sistema.obter_resultado())
        elif opcao == "0":
            print("Encerrando o programa. Até logo!")
            break
        else:
            print("Opção inválida! Tente novamente.")


menu()