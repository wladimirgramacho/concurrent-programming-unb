**1 Defina processos e threads, abordando suas principais diferencas.**

<b style="color: #4e90dc">Processos:</b> programas em execução constituidos por código executável, pilha de execução, estado do processo, prioridade do processo, valor do contador do programa (PC), valor do apontador de pilha (registrador $sp), valores dos demais registradores.

<div style="border: 1px solid #4e90dc; padding: 1em;">
Exemplo processo x programa: preparação de bolo de aniversário
  <ul>
    <li> A receita – programa (algoritmo) </li>
    <li> Os ingredientes – dados de entrada </li>
    <li> O cozinheiro – processador  </li>
    <li> Atividade de preparar o bolo (processo) </li>
  </ul>
</div>


<b style="color: #4e90dc">Threads:</b> são fluxos de execução de um processo. Frequentemente, há situações em que é desejável ter múltiplos threads de controle no mesmo espaço de endereçamento executando em pseudo paralelo, como se fossem processos separados. Logo, podemos ter várias threads rodando no mesmo processo.


**2  Defina processos concorrentes.**

<b style="color: #4e90dc">Processos concorrentes:</b> são processos que concorrem por recursos compartilhados e são executados simultaneamente em tempo lógico. Esses processos utilizam memória compartilhada ou troca de mensagens para se comunicar entre si.

**3  Dê as definições de condicoes de corrida, regiao crıtica e exclusao mutua.**

Processos podem ser independentes ou cooperativos. 


<b style="color: #4e90dc">Condições de corrida:</b> nos processos cooperativos, condições de corrida são situações onde dois ou mais processos estão acessando dados compartilhados e o resultado final depende de quem executa e quando executa.

<b style="color: #4e90dc">Região crítica:</b> parte do código dos processos onde é feito o acesso à memória compartilhada.

<b style="color: #4e90dc">Exclusão mútua:</b> impedir que dois ou mais processos acessem um mesmo recurso ao mesmo tempo.

**4  Defina deadlock e starvation.**

<b style="color: #4e90dc">Deadlock:</b> situação onde dois processos ficam impedidos de continuar suas execuções e não conseguem se destravar, ficando bloqueados.

<b style="color: #4e90dc">Starvation:</b> situação na qual um processo nunca é executado, pois outros processos de maior prioridade (ou maior volume) impedem o processo de executar.

**5  Explique como varias threads conseguem acessar um mesmo dado compartilhado e indique como esta funcionalidade pode ser fornecida para processos.**

Para que várias threads consigam acessar um mesmo dado compartilhado, utilizamos locks. <b style="color: #4e90dc">Locks</b> são formas de delimitar uma região crítica de forma a garantir a exclusão mútua entre as threads. Ao chegar na região crítica, a thread fecha o lock, fazendo com que qualquer outra thread que chegue ali pare sua execução e aguarde o lock ser liberado. Após executar a região crítica, a thread libera o lock para que outras threads possam entrar na região crítica.

Na linguagem C, pode-se utilizar a biblioteca pthreads para utilizar locks.

**6  Os algoritmos abaixo representam dois processos que acessam duas variaveis compartilhadas (x e y) e utilizam dois locks para controlar este acesso.  Identifique os possıveis problemas presentes nestes algoritmos e proponha novos algoritmos que facam as mesmas tarefas de forma correta.**
```c
// variáveis compartilhadas                      
int x = 0;
int y = 0;
lock lx;
lock ly;
P1() {                               P2(){
lock(lx);                            lock(ly);
x=x+1;                               y=y+1;
lock(ly);                            lock(lx);
x=x+y;                               y=y+x;
unlock(lx);                          unlock(ly);
unlock(ly);                          unlock(lx);
}

```
Prontamente vemos que esse código gera um deadlock entre os processos. Se P1 chegar no `lock(ly)` e P2 estiver no `lock(lx)`, os processos ficarão bloqueados. Uma possível solução seria:

```c
// variáveis compartilhadas                      
int x = 0;
int y = 0;
lock lx;
lock ly;
P1() {                               P2(){
lock(lx);                            lock(ly);
x=x+1;                               y=y+1;
unlock(lx)                           unlock(ly);
lock(lx);                            lock(lx);
lock(ly);                            lock(ly);
x=x+y;                               y=y+x;
unlock(lx);                          unlock(lx);
unlock(ly);                          unlock(ly);
}

```
Dessa forma, não temos deadlock.

**7 questão era de matéria não estudada**

**8  Dado o algoritmo de chaveamento obrigatorio (ver abaixo) para implementacao de exclusao mutua (lock) entre dois processos, responda as seguintes questoes.
a) (1.0 ponto) - Cite e explique um problema relacionado com este algoritmo.
b) (1.0 ponto) - Explique como o mesmo poderia ser estendido para implementar um lock entre três ou mais processos.**
```c
P0() {
  while (TRUE) {
    while (turn != 0) { /*espera*/;
    regiao_critica();
    turn = 1;
    regiao_nao_critica();
    }
  }
}
P1() {
  while (TRUE) {
    while (turn != 1) {  /*espera */;
      regiao_critica();
      turn = 0;
      regiao_nao_critica();
    }
  }
}
```
a) Se um dos processos é muito mais lento que o outro, o algoritmo de chaveamento obrigatório pode não ser uma boa ideia, pois nesta solução deve haver, obrigatoriamente, a entrada alternada dos dois processos em suas regiões críticas, podendo haver uma perda no desempenho.

TEM QUE FAZER AINDA

**9  Resolva o seguinte problema utilizando qualquer mecanismo estudado:  “Existe um galpao com comida (considere que a comida e infinita) para caes e gatos.  Considere que um numero qualquer (maior do que zero) de caes e somente um gato utiliza este galpao para se alimentar.  Os caes entram no galpao e ficam la comendo ate que o gato desejar comer.  Neste caso, os caes devem sair do galpao de forma que o gato entra no galpao apenas quando o mesmo estiver sem caes em seu interior.  Diferente dos caes, o gato permanece comendo por um tempo determinado e depois sai do galpao.  Enquanto o gato estiver se alimentando, os caes nao podem entrar no galpao.”**

TEM QUE FAZER AINDA

```c
caes(){
  while(TRUE){
    
  }
}

gatos(){
  while(TRUE){
    
  }

}
```