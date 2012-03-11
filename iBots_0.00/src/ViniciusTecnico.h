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

class ViniciusTecnico {
public:
    
    /*! Referencia para a visão de mundo */
    const GlobalWorldModel& mundo;
    /*! true se a bola pertence ao nosso time */
    bool bolaNossa;
    /*! Número de aliados no centro de jogo */
    int aliadosCentroJogo;
    /*! Número de oponentes no centro de jogo */
    int oponentesCentroJogo;
    
    ViniciusTecnico(const GlobalWorldModel& mundoArg):mundo(mundoArg) {
        bolaNossa = false;
        aliadosCentroJogo = 0;
        oponentesCentroJogo = 0;
    }
    
    virtual ~ViniciusTecnico() {}
    
    void executar() {
        bolaNossa = bolaEhNossa();
        aliadosCentroJogo = numJogadoresCentroJogo(mundo.teammates());
        oponentesCentroJogo = numJogadoresCentroJogo(mundo.opponents());
        
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
     a trave adversária.
     \Argumentos
     jogadores - uma lista contendo os jogadores que se deseja saber se então
     no centro de jogo.
     \retorno
     O numero de jogadores no centro de jogo
     
     Foi verificado que é como se sempre o time fosse o da esquerda,
     ou seja ataca da esquerda para a direita.
    */
    int numJogadoresCentroJogo(
                const std::vector<const GlobalPlayerObject*>& jogadores)
    {
        int num = 0;
        
        for (std::vector<const GlobalPlayerObject*>::const_iterator
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
    bool jogadorCentroJogo(const GlobalPlayerObject* jogador) {
        if(jogador->pos().x >= mundo.ball().pos().x) {
            return true;
        } else return false;
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
                << oponentesCentroJogo;
        
        escreve << "|" << std::endl;
        escreve.close();
    }
    
};

#endif	/* VINICIUSTECNICO_H */

