# building_monitor_system

Aluno: Maicon Lucas Mares<br>
Matrícula: 180023411

## Como utilizar
- Para evitar erros na ordem das conexões entre sockets é necessário compilar e executar o servidor central. Para isso execute:
```bash
make
make run
```

- Após feito, compile e rode os servidores distribuídos. É necessário informar o nome do arquivo de configuração (não o caminho completo):
```bash
make
make run FILENAME=configuracao_andar_1.json
```
```bash
make
make run FILENAME=configuracao_andar_terreo.json
```
**Obs.:** Pode ser necessário limpar a pasta de binários e de objetos antes de compilar algum dos servidores, para isto rode `make clean`.

![Screenshot from 2022-04-04 23-46-44](https://user-images.githubusercontent.com/47460478/161669040-0e3d0b57-de53-4930-a64f-703ed7652ec9.png)
