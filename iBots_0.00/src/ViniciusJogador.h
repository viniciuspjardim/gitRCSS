// -*-c++-*-

/* 
 * File:   ViniciusJogador.h
 * Author: vinicius
 *
 * Created on February 29, 2012, 2:29 AM
 */

#ifndef VINICIUSJOGADOR_H
#define	VINICIUSJOGADOR_H

#include <rcsc/player/world_model.h>
#include <rcsc/action/body_kick_one_step.h>
#include <rcsc/player/player_agent.h>
#include "bhv_basic_offensive_kick.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace rcsc;

class ViniciusJogador {
public:
    
    enum Dist{MUITO_PERTO, PERTO, MEDIA, LONGE};
    enum Estado{ATAQUE, DEFESA, CONTRA_ATAQUE, CONTRA_DEFESA};
    
    /*! Referência para a visão de mundo */
    const WorldModel& mundo;
    /*! Referência para o joador atual*/
    PlayerAgent& eu;
    /*! true se a bola pertence ao nosso time */
    bool bolaNossa;
    /*! true se a bola pertence a self*/
    bool bolaMinha;
    /*! true se o jogador está no centro de jogo */
    bool noCentroJogo;
    /*! Número de aliados no centro de jogo */
    int aliadosCentroJogo;
    /*! Número de oponentes no centro de jogo */
    int oponentesCentroJogo;
    
    /*! Estado do jogo*/
    Estado estadoAtual;
    
    ViniciusJogador(const WorldModel& mundoArg, PlayerAgent& euArg)
                                :mundo(mundoArg), eu(euArg) {
        bolaNossa = false;
        bolaMinha = false;
        noCentroJogo = false;
        aliadosCentroJogo = 0;
        oponentesCentroJogo = 0;
        
        estadoAtual = DEFESA;
    }
    
    virtual ~ViniciusJogador() {}

    void executar() {
        bolaNossa = bolaEhNossa();
        bolaMinha = mundo.self().isKickable();
        noCentroJogo = jogadorCentroJogo(mundo.self(), 0.0, 0.0);
        aliadosCentroJogo = numJogadoresCentroJogo(mundo.teammates(), 0.0, 0.0);
        oponentesCentroJogo = numJogadoresCentroJogo(mundo.opponents(), 0.0, 0.0);
        
        atualizarEstado();
        
        fazerLog();
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
        bool nossaAreaChute = mundo.existKickableTeammate();
        bool areaChuteDeles = mundo.existKickableOpponent();
         
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
                const PlayerCont& jogadores, double yIni, double yFim)
    {
        int num = 0;
        
        for(PlayerCont::const_iterator
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
                const AbstractPlayerObject& jogador, double yIni, double yFim)
    {
        bool ret = false;
        if(yIni < yFim) {
            if(jogador.pos().y >= yIni &&
                        jogador.pos().y <= yFim &&
                        jogador.pos().x >= mundo.ball().pos().x)
            {
                ret = true;
            }
        }
        else {
            if(jogador.pos().x >= mundo.ball().pos().x) {
                ret = true;
            }
        }
        return ret;
    }
    
    /*!\brief retorna true se o time oponente respeita o principio da contenção.
     Ou seja, há um jogador oponente marcando o atacante que está com a bola.
    */
    bool contencao() {
        const PlayerPtrCont& oponentes = mundo.opponentsFromBall();
        
        for(PlayerPtrCont::const_iterator
                it = oponentes.begin(),
                end = oponentes.end();
                it != end;
                ++it)
        {
            Dist d = calcDistancia((*it)->pos(), mundo.ball().pos());
            if((d == PERTO || d == MUITO_PERTO) &&
                    jogadorCentroJogo(*(*it), 0.0, 0.0)) {
                return true;
            }
        }
        return false;
    }
    
    enum Dist calcDistancia(const Vector2D& p1, const Vector2D& p2) {
        if(p1.dist(p2) <= 2.0) return MUITO_PERTO;
        if(p1.dist(p2) <= 4.0) return PERTO;
        if(p1.dist(p2) <= 8.0) return MEDIA;
        if(p1.dist(p2) >  8.0) return LONGE;
    }

    void fazerLog() {
        int unum = mundo.self().unum();
        std::stringstream stream;
        stream << "/home/vinicius/gitRCSS/iBots_0.00/logs/" << unum << ".txt";
        const char* arq;
        arq = stream.str().c_str();
        
        std::ofstream escreve;
        escreve.open(arq, std::ios::trunc);
        
        // Escreve o número do jogador
        escreve << unum << "\t|";
        // N de bola nossa, D de deles
        if(bolaNossa) {
            escreve << "V|";
        } else {
            escreve << "F|";
        }
        // Centro de jogo: numAliados-numOponentes
        escreve << aliadosCentroJogo << "-"
                << oponentesCentroJogo << "|";
        // C se o jogador estiver no centro de jogo, F fora
        /*
        if(noCentroJogo) {
            escreve << "V|";
        } else {
            escreve << "F|";
        }
        
        // Calcula a distancia que este jogador está da bola
        Dist a  = calcDistancia(mundo.self().pos(), mundo.ball().pos());
        if(a == MUITO_PERTO) escreve << "MP";
        else if(a == PERTO) escreve << "P";
        else if(a == MEDIA) escreve << "M";
        else if(a == LONGE) escreve << "L";
        else escreve << "?";
        */
        
        if(estadoAtual == ATAQUE) escreve << "A";
        else if(estadoAtual == DEFESA) escreve << "D";
        else if(estadoAtual == CONTRA_ATAQUE) escreve << "CA";
        else if(estadoAtual == CONTRA_DEFESA) escreve << "CD";
        
        escreve << "|" << std::endl;
        escreve.close();
    }
};

#endif	/* VINICIUSJOGADOR_H */