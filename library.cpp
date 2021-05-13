/*
Created on Thursday may 5 17:37:14 2021
@author: Lucas Victor Silva Pereira
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

#define MAXID 10000

class Livro
{
	private:
		char ISBN[14];
		char titulo[101];
		char autor[501];
		char editora[81];
		char ano[5];
		char cidade[81];
		char pais[51];
		char edicao[3];

	public:
		void make() // Preenche informações do livro
		{
			cout << "ISBN:" << endl;
			cin.ignore();
			cin.getline(ISBN,14);
			cout << "Titulo:" << endl;
			cin.getline(titulo,101);
			cout << "Autores (separados por \";\"):" << endl;
			cin.getline(autor,501);
			cout << "Editora:" << endl;
			cin.getline(editora,81);
			cout << "Ano:" << endl;
			cin.getline(ano,5);
			cout << "Cidade:" << endl;
			cin.getline(cidade,81);
			cout << "Pais" << endl;
			cin.getline(pais,51);
			cout << "Edicao" << endl;
			cin.getline(edicao,3);
		}
		void make(char *ch) // Preenche informações do livro
		{
			strcpy(ISBN,ch);
			cout << "Titulo:" << endl;
			cin.getline(titulo,101);
			cout << "Autores (separados por \";\"):" << endl;
			cin.getline(autor,501);
			cout << "Editora:" << endl;
			cin.getline(editora,81);
			cout << "Ano:" << endl;
			cin.getline(ano,5);
			cout << "Cidade:" << endl;
			cin.getline(cidade,81);
			cout << "Pais" << endl;
			cin.getline(pais,51);
			cout << "Edicao" << endl;
			cin.getline(edicao,3);
		}
		char* getISBN() // retorna a posição da variável que contém o ISBN do livro
		{
			return &ISBN[0];
		}
		void print() // imprime na tela as informações do livro
		{
			cout << "ISBN: " << ISBN << "\n";
			cout << "Titulo: " << titulo << "\n";
			cout << "Autores: " << autor << "\n";
			cout << "Editora: " << editora << "\n";
			cout << "Ano: " << ano << "\n";
			cout << "Cidade: " << cidade << "\n";
			cout << "Pais: " << pais << "\n";
			cout << "Edicao: " << edicao << endl;
		}
};

class Elemento
{
	friend class Lista;

	private:
		char ISBN[14];
		unsigned long int id;
};

class Lista
{
	private:
		Elemento* e;
		int ult;
		int max;

	public:
		Lista(int tam=MAXID)
		{
			max = tam;
			ult= -1;
			e = new Elemento[max];
		}
		~Lista()
		{
			delete []e;
		}

		bool add(unsigned long int id, char* ISBN) // Adiciona elemento na lista, de forma ordenada.
		{
			if(full()) return false;
			if(!empty())
			{
				for(int i=ult; i>=0; i--)
				{
					if(strcmp(ISBN,e[i].ISBN) <0)
					{
						strcpy(e[i+1].ISBN,e[i].ISBN);
						e[i+1].id = e[i].id;
					}
					else
					{
						strcpy(e[i+1].ISBN,ISBN);
						e[i+1].id = id;
						ult++;
						return true;
					}
					
				}
			}			
			else
			{
				e[ult+1].id = id;
				strcpy(e[ult+1].ISBN, ISBN);
				ult++;
			}
			return true;
		}
		bool clr(int pos) // Retira da lista o elemento na posição pos
		{
			if (pos > ult || pos < 0) return false;
			for(int i=pos; i<ult; i++)
			{
				e[i].id = e[i+1].id;
				strcpy(e[i].ISBN, e[i+1].ISBN);
			}
			ult--;
			cout << ult << endl;
			return true;
		}
/*		void sort()
		{
			Desnecessário devido ao manejo da lista.
			Sempre que um novo registro é adicionado, ele entra ordenado na lista e ao final do arquivo, quando o programa fecha o arquivo é reescrito conforme a ordem na lista.
			Dessa forma o arquivo está sempre ordando quando o programa é encerrado.
		}
*/		//implementar busca binária - ok
		int srch(char* ch, int opt =-1, int iniS =0, int ultS =0) // busca, binária, na lista a posição do livro de ISBN ch
		{
			if(opt == -1)
			{
				iniS=0;
				ultS=ult;
			}
			if(iniS>ultS) return -1;
///*DEBUG*/			cout << iniS << endl;
			int meio = (iniS+ultS)/2;
//DEBUG			cout << meio << endl;
//DEBUG			cout << e[meio].ISBN << endl;
			int res= strcmp(e[meio].ISBN,ch);
//DEBUG			cout << res << endl;
			if(res == 0) return meio;
			if(res <  0) return srch(ch, 1, meio+1, ultS);
			return srch(ch, 1, iniS, meio-1);

/*			for(int i=0; i<=ult; i++)
				if(strcmp(e[i].ISBN, ch) ==0) return i;
			return -1;
*/		}
		bool full() // lista cheia?
		{
			if(ult >= max) return true;
			return false;
		}
		bool empty() // lista vazia?
		{
			if(ult != -1) return false;
			return true;
		}
		int size() // retorna tamanho da lista
		{
			return ult+1;
		}
		void print() // imprime todo ISBN da lista
		{
			if(empty()) return;
			for(int i=0; i<=ult; i++)
				cout << i+1 << ") ISBN: " << e[i].ISBN << endl; 
			return;
		}
		int posArq(char* ch) // pesquisa na lista o livro de ISBN ch e retorna sua posição no arquivo
		{
			int pos = srch(ch);
			if(pos == -1) return -1;
			return e[pos].id;
		}
		unsigned long int posArq(int p) // retorna a posição no arquivo do livro na posição p da lista
		{
			if(p<0 || p>ult) return -1;
			return e[p].id;
		}
};

class BD
{
//	private:
	public:
		bool failFlag;
		char nomeArq[31];
		fstream arq;
		Lista index;

		bool posLt(char* ch)
		{
			int pos = index.posArq(ch);
			if(pos == -1)
			{
				arq.seekg(0, ios::end);
				return false;
			}
			arq.seekg(pos, ios::beg);
			return true;
		}
		bool posEs(char* ch)
		{
			int pos = index.posArq(ch);
			if(pos == -1)
			{
				arq.seekp(0, ios::end);
				return false;
			}
			arq.seekp(pos, ios::beg);
			return true;
		}

	public:
		BD(char* ch)
		{
			cout << "Abrindo o arquivo ";
			strcpy(nomeArq, ch);
			cout << nomeArq << "\n\n";
			arq.clear();
			arq.open(nomeArq, ios::out | ios::app | ios::binary); // caso não exista, cria o arquivo. O arquivo não é apagado (conteúdo) caso existir
			arq.close();
			arq.open(nomeArq, ios::in | ios::out | ios::binary); // abre o arquivo para a execução do programa.
//DEBUG			arq.open("lib.dat", ios::binary);
			if(arq.fail())
			{
				cout <<"Falha ao abrir o arquivo" << endl;
				failFlag = true;
			}
			else
			{
				failFlag = false;
				arq.seekg(0,ios::beg);
				char temp[14];
				unsigned long int aux;
				unsigned long int sz;
				arq.seekg(0,ios::end);
				sz = arq.tellg();
				if(aux <= 0) return;
//DEBUG				cout << "size*: " << sz << endl;
				sz /= sizeof(Livro); // Calcula quantos livros existem no arquivo
//DEBUG				cout << "size: " << sz << endl;
				for(int j=0; j<sz; j++) // realiza a leitura dos livros no arquivo
				{
					arq.seekg(j*sizeof(Livro),ios::beg);
					aux=arq.tellg();
					arq.read(&temp[0],14*sizeof(char));
					index.add(aux, &temp[0]);
				}
/* tentativa falha				while(!arq.eof())
				{
					arq.read((char*)&temp, 14*sizeof(char));
					index.add(arq.tellg(), &temp[0]);
					aux++;
					arq.seekg(aux*sizeof(Livro), ios::beg);
				}
*/
			}
		}
		~BD()
		{
			fstream buffer;
			buffer.open("buffer.dat",ios::out | ios::binary);
			int sz=index.size();
			Livro entry;
			char auxC[14];
			for(int i=0; i<sz; i++) // copia os livros ainda na lista para um arquivo temporário
			{
				arq.seekg(index.posArq(i), ios::beg);
				arq.read((char*)&entry, sizeof(Livro));
				buffer.write((char*)&entry, sizeof(Livro));
			}
			char sys[51];
			//Deleta o arquivo original e renomeia o temporário.
			strcpy(sys,"rm ");
			strcat(sys,nomeArq);
			system(sys);
			strcpy(sys, "mv buffer.dat ");
			strcat(sys, nomeArq);
			system(sys);

//tentativas falhas
//			system("rm "+nomeArq);
//			system("mv buffer.dat "+nomeArq);
/*			int tam = index.size();
			Livro temp[tam];
			for(int i = 0; i<tam; i++)
			{
				arq.seekg(index.posArq(i), ios::beg);
				arq.read((char*)&temp[i], sizeof(Livro));
			}
			arq.close();
			arq.open(nomeArq, ios::out | ios::binary);
			arq.write((char*)&temp, tam*sizeof(Livro)); //Teste ecreve todo o arranjo de uma só vez
			arq.close();
*/
		}

		bool add(Livro &reg) // Adiciona novo livro ao arquivo e lista
		{
			char* temp = reg.getISBN();
			if(index.srch(temp) != -1) return false;
			arq.seekp(0, ios::end);
			int pos = arq.tellp();
			arq.write((char*)&reg, sizeof(Livro));
			index.add(pos, temp);
			return true;
		}
		bool altera(Livro &reg) // Atualiza registro do livro no registro
		{
			if(!posEs(reg.getISBN())) return false;
			arq.write((char*)&reg, sizeof(Livro));			
			return true;
		}
		bool clr(char* ch) // Retira livro de ISBN ch da lista
		{
			int pos = index.srch(ch);
			if(pos == -1) return false;
			cout << "Registro encontrado em " << pos << endl;
			index.clr(pos);
			return true;
		}
		bool obtem(char* ch, Livro &reg) //  Lê do arquivo o livro de ISBN ch e salva-o em reg
		{
			if(!posLt(ch)) return false;
			arq.read((char*)&reg ,sizeof(Livro));
			return true;
		}

		int size() // retorna tamanho da lista
		{
			return index.size();
		}

		void print() // imprime todo ISBN da lista
		{
			index.print();
			return;
		}

		void print(char* ch) // imprime as informações do livro de ISBN ch presente na lista
		{
			Livro entry;
			if(!obtem(ch, entry))
			{
				cout << "Registro nao encontrado." << endl;
				return;
			}
			entry.print();
		}
		bool failled() // indica falha de abertura do arquivo
		{
			return failFlag;
		}
};

int main()
{
	int opt;
	char db[31];
	cout << "Digite o nome do arquivio banco de dados: ";
	cin.getline(db,31);
	BD acervo(&db[0]);
	if(acervo.failled()) return 0; // Para o programa em caso de falha na abertura do arquivo
	int qtd;

	while(true)
	{
		qtd = acervo.size();
		cout << "Existem " << qtd << " livros atualmente cadastrados no registro." << endl;

		acervo.print();

		cout << "Gerenciamento:\n1 - Incluir\n2 - Alterar\n3 - Excluir\n4 - Consulta\n5 - Finalizar" << endl;
		cout << "Opcao  ";
		cin >> opt;
		cout << endl;
		Livro entry;
		char isbn[14];

		switch(opt)
		{
			case 1:
				entry.make();
				acervo.add(entry);
				qtd = acervo.size();
				break;

			case 2:
				cout << "Qual registro deseja alterar? ";
				cin.ignore();
				cin.getline(isbn, 14);
				cout << "Registro atual:" << endl;
				acervo.print(isbn);
				cout << "Entre com o novo registro (completo)." << endl;
				entry.make(isbn);
				acervo.altera(entry);
				break;

			case 3:
				cout << "Qual registro desja excluir? ";
				cin.ignore();
				cin.getline(isbn,14);
				if(!acervo.clr(isbn)) cout << "Registro nao encontrado" << endl;
				qtd = acervo.size();
				break;

			case 4:
				cout << "Sobre qual registro deseja saber? ";
				cin.ignore();
				cin.getline(isbn, 14);
				cout << "Localisando: " << isbn << endl;
				acervo.print(isbn);
				cout << endl;
				break;

			case 5:
				return 0;
		}
	}
}