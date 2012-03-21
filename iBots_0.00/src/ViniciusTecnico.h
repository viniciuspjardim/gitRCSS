/* 
 * File:   ViniciusTecnico.h
 * Author: vinicius
 *
 * Created on March 5, 2012, 3:47 PM
 */

#ifndef VINICIUSTECNICO_H
#define	VINICIUSTECNICO_H

#include <rcsc/coach/global_world_model.h>
#include <rcsc/common/server_param.h>
#include <rcsc/common/player_type.h>

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace rcsc;

class DistComparator
        : public std::binary_function< const GlobalPlayerObject *,
                                       const GlobalPlayerObject *,
                                       bool > {
public:
    
    const Vector2D& p;
    
    /*!
     Compara dois GlobalPlayerObjects relativo a distancia do ponto p
     */
    DistComparator(const Vector2D& vArg):p(vArg) {}
    
    /*!
      \brief  compara pela distancia da bola
      \param lhs variavel da esquerda
      \param rhs variavel da direita
      \return resultado da comparação
     */
    result_type operator()( first_argument_type lhs,
                            second_argument_type rhs ) const
    {
        return lhs->pos().dist(p) < rhs->pos().dist(p);
        //return lhs->distFromBall() < rhs->distFromBall();
    }
};

class ViniciusTecnico {
public:
    
    enum Dist{MUITO_PERTO, PERTO, MEDIA, LONGE};
    enum Estado{ATAQUE, DEFESA, CONTRA_ATAQUE, CONTRA_DEFESA};
    
    /*! Referencia para a visão de mundo */
    const GlobalWorldModel& mundo;
    /*! true se a bola pertence ao nosso time */
    bool bolaNossa;
    /*! Número de aliados no centro de jogo */
    int aliadosCentroJogo;
    /*! Número de oponentes no centro de jogo */
    int oponentesCentroJogo;
    
    /*! Estado do jogo*/
    Estado estadoAtual;
    
    ViniciusTecnico(const GlobalWorldModel& mundoArg):mundo(mundoArg) {
        bolaNossa = false;
        aliadosCentroJogo = 0;
        oponentesCentroJogo = 0;
        
        estadoAtual = DEFESA;
    }
    
    virtual ~ViniciusTecnico() {}
    
    void executar() {
        bolaNossa = bolaEhNossa();
        aliadosCentroJogo = numJogadoresCentroJogo(mundo.teammates(), 0.0, 0.0);
        oponentesCentroJogo = numJogadoresCentroJogo(mundo.opponents(), 0.0, 0.0);
        
        atualizarEstado();
        
        fazerLog();
    }
    
    /*!\brief retorna true se pelo menos um dos jogadores passados no vetor alcança a bola.*/    
    bool existeJogadorChutavel(const std::vector<const GlobalPlayerObject*>& jogadores) {
        for (std::vector<const GlobalPlayerObject*>::const_iterator
                it = jogadores.begin(),
                end = jogadores.end();
                it != end;
                ++it)
        {
            int type = mundo.playerTypeId((*it)->side(), (*it)->unum());
            const PlayerType * param = PlayerTypeSet::i().get(type);
            double kickable_area = (param
                    ? param->kickableArea()
                    : ServerParam::i().defaultKickableArea());

            if ((*it)->pos().dist(mundo.ball().pos()) < kickable_area) {
                return true;
            }
        }

        return false;
    }
    
    /*!
     \brief retorna um vetor de jogadores ordenado de forma crescente relativo
     a distancia do ponto p.
     */
    const std::vector<const GlobalPlayerObject*>*
    jogadoresPertoP(const std::vector<const GlobalPlayerObject*>& jogadores,
                        const Vector2D& p)
    {
        std::vector<const GlobalPlayerObject*>* jOrdenados =
                new std::vector<const GlobalPlayerObject*>();
        
        for (std::vector<const GlobalPlayerObject*>::const_iterator
                it = jogadores.begin(),
                end = jogadores.end();
                it != end;
                ++it)
        {
            jOrdenados->insert(jOrdenados->end(), (*it));
        }
        
        std::sort(jOrdenados->begin(),
                jOrdenados->end(),
                DistComparator(p)
        );
        
        return jOrdenados;
    }
    
    void atualizarEstado() {
        if(bolaNossa) {
            // Se o time oponente não respeita o principio da contenção e
            // o nosso time está em igualdade ou superioridade numérica
            // (excluindo o goleiro adversário) considerar como contra-ataque
            if(!contencao() && aliadosCentroJogo >= oponentesCentroJogo -1 &&
                    mundo.ball().pos().x >= 46.0)
            {
                estadoAtual = CONTRA_ATAQUE;
            } else {
                estadoAtual = ATAQUE;
            }
        } else {
            estadoAtual = DEFESA;
        }
    }
    
    /*!
      \brief
     <b>Caso a bolaNossa = true:</b>
     -Retorna falso se apenas jogadores adversários alcançarem a bola.
     -Em todos os outros casos retorna verdadeiro.
     
     <b>Caso a bolaNossa = false:</b>
     -Retorna verdadeiro se somente jogadores do nosso time alcançar a bola;
     -Em todos os outros casos reorna falso.
    */
    bool bolaEhNossa() {
        bool nossaAreaChute = existeJogadorChutavel(mundo.teammates());
        bool areaChuteDeles = existeJogadorChutavel(mundo.opponents());
        
         
        if(bolaNossa) {
            return  !(areaChuteDeles && !nossaAreaChute);
        }
        else {
            return (nossaAreaChute && !areaChuteDeles);
        }
    }
    
    /*!
     \brief
     Conta o numero de jogadores depois da bola, ou seja, entre a bola e 
     a trave adversária (profundidade).
     \Argumentos
     jogadores - uma lista contendo os jogadores que se deseja saber se então
     no centro de jogo.\n
     yIni - inicio do corredor que se deseja limitar em largura\n
     yFim - fim do corredor que se deseja limitar em largura\n
     jogadores que tem sua posição em y fora desse intervalo não serão contados.
     Para não delimitar um corredor em y (largura) pode-se passar yIni igual a
     yFim
     \retorno
     O numero de jogadores no centro de jogo
     
     Foi verificado que é como se sempre o time fosse o da esquerda,
     ou seja ataca da esquerda para a direita.
    */
    int numJogadoresCentroJogo(
                const std::vector<const GlobalPlayerObject*>& jogadores,
                double yIni, double yFim)
    {
        int num = 0;
        
        for (std::vector<const GlobalPlayerObject*>::const_iterator
                it = jogadores.begin(),
                end = jogadores.end();
                it != end;
                ++it)
        {
            if(jogadorCentroJogo(*it, yIni, yFim)) num++;
        }
        
        return num;
    }
    
    /*!
     \brief retorna true se um dado jogador está no centro de jogo.
    */
    bool jogadorCentroJogo(
                const GlobalPlayerObject* jogador, double yIni, double yFim)
    {
        bool ret = false;
        if(yIni < yFim) {
            if(jogador->pos().y >= yIni &&
                        jogador->pos().y <= yFim &&
                        jogador->pos().x >= mundo.ball().pos().x)
            {
                ret = true;
            }
        }
        else {
            if(jogador->pos().x >= mundo.ball().pos().x) {
                ret = true;
            }
        }
        return ret;
    }
    
    bool contencao() {
        
        const std::vector<const GlobalPlayerObject*>* oponentes = 
                jogadoresPertoP(mundo.opponents(), mundo.ball().pos());
        
        for(std::vector<const GlobalPlayerObject*>::const_iterator
                it = oponentes->begin(),
                end = oponentes->end();
                it != end;
                ++it)
        {
            Dist d = calcDistancia((*it)->pos(), mundo.ball().pos());
            if(d == PERTO || d == MUITO_PERTO) {
                delete(oponentes);
                return true;
            }
        }
        delete(oponentes);
        return false;
    }
    
    enum Dist calcDistancia(const Vector2D& p1, const Vector2D& p2) {
        if(p1.dist(p2) <= 2.0) return MUITO_PERTO;
        if(p1.dist(p2) <= 4.0) return PERTO;
        if(p1.dist(p2) <= 8.0) return MEDIA;
        if(p1.dist(p2) >  8.0) return LONGE;
    }
    
    void fazerLog() {
        std::stringstream stream;
        stream << "/home/vinicius/gitRCSS/iBots_0.00/logs/coach.txt";
        const char* arq;
        arq = stream.str().c_str();
        
        std::ofstream escreve;
        escreve.open(arq, std::ios::trunc);
        
        // Escreve o número do jogador
        escreve << "coach" << "\t|";
        // N de bola nossa, D de deles
        if(bolaNossa) {
            escreve << "V|";
        } else {
            escreve << "F|";
        }
        // Centro de jogo: numAliados-numOponentes
        escreve << aliadosCentroJogo << "-"
                << oponentesCentroJogo << "|";
        
        if(estadoAtual == ATAQUE) escreve << "A";
        else if(estadoAtual == DEFESA) escreve << "D";
        else if(estadoAtual == CONTRA_ATAQUE) escreve << "CA";
        else if(estadoAtual == CONTRA_DEFESA) escreve << "CD";
        
        escreve << "|" << std::endl;
        escreve.close();
    }
    
};

#endif	/* VINICIUSTECNICO_H */

