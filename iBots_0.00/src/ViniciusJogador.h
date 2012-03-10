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

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace rcsc;

class ViniciusJogador {
public:
    
    /*! Referencia para a visão de mundo */
    const WorldModel& mundo;
    /*! true se a bola pertence ao nosso time */
    bool bolaNossa;
    /*! true se o jogador está no centro de jogo */
    bool noCentroJogo;
    /*! Número de aliados no centro de jogo */
    int aliadosCentroJogo;
    /*! Número de oponentes no centro de jogo */
    int oponentesCentroJogo;
    
    
    
    ViniciusJogador(const WorldModel& mundoArg):mundo(mundoArg) {
        bolaNossa = false;
        noCentroJogo = false;
        aliadosCentroJogo = 0;
        oponentesCentroJogo = 0;
    }
    
    virtual ~ViniciusJogador() {}

    void executar() {
        mundo.self();
        bolaNossa = bolaEhNossa();
        noCentroJogo = jogadorCentroJogo(mundo.self());
        aliadosCentroJogo = numJogadoresCentroJogo(mundo.teammates());
        oponentesCentroJogo = numJogadoresCentroJogo(mundo.opponents());
        
        fazerLog();
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
     a trave adversária.
     \Argumentos
     jogadores - uma lista contendo os jogadores que se deseja saber se então
     no centro de jogo.
     \retorno
     O numero de jogadores no centro de jogo
     
     Foi verificado que é como se sempre o time fosse o da esquerda,
     ou seja ataca da esquerda para a direita.
    */
    int numJogadoresCentroJogo(const PlayerCont& jogadores) {
        int num = 0;
        
        for(PlayerCont::const_iterator
                it = jogadores.begin(),
                end = jogadores.end();
                it != end;
                ++it)
        {
            if(jogadorCentroJogo(*it)) num++;
        }
        
        return num;
    }
    
    /*!
     \brief retorna true se um dado jogador está no centro de jogo.
    */
    bool jogadorCentroJogo(const AbstractPlayerObject& jogador) {
        if(jogador.pos().x >= mundo.ball().pos().x) {
            return true;
        } else return false;
    }

    void fazerLog() {
        int unum = mundo.self().unum();
        std::stringstream stream;
        stream << "/home/vinicius/gitRCSS/iBots_0.00/logs/" << unum << ".txt";
        const char* arq;
        arq = stream.str().c_str();
        
        std::ofstream escreve;
        escreve.open(arq, std::ios::trunc);
        
        escreve << unum << "\t";
        // N de bola nossa, D de deles
        if(bolaNossa) {
            escreve << "N|";
        } else {
            escreve << "D|";
        }
        // C se o jogador estiver no centro de jogo, F fora
        if(noCentroJogo) {
            escreve << "C|";
        } else {
            escreve << "F|";
        }
        escreve << aliadosCentroJogo << "-"
                << oponentesCentroJogo << "|" << std::endl;
        
        escreve.close();
    }
};

#endif	/* VINICIUSJOGADOR_H */