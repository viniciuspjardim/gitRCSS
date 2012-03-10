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
    
    bool bolaNossa;
    
    const GlobalWorldModel& mundo;
    
    ViniciusTecnico(const GlobalWorldModel& mundoArg):mundo(mundoArg) {
        bolaNossa = false;
    }
    
    virtual ~ViniciusTecnico() {}
    
    void executar() {
        bolaNossa = bolaEhNossa();
        logPosseBola();
    }

    /*!\brief retorna true se pelo menos um dos jogadores passados no vetor alcança a bola.*/    
    bool existeJogadorChutavel(const std::vector<const GlobalPlayerObject*>& jogadores) {
        for (std::vector<const GlobalPlayerObject*>::const_iterator
                it = jogadores.begin(),
                end = jogadores.end();
                it != end;
                ++it) {
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
    
    void logPosseBola() {
        std::stringstream stream;
        stream << "/home/vinicius/gitRCSS/iBots_0.00/logs/coach.txt";
        const char* arq;
        arq = stream.str().c_str();
        
        std::ofstream escreve;
        escreve.open(arq, std::ios::trunc);
        
        escreve << "coach" << "\t";
        if(bolaNossa) {
            escreve << "nossa" << std::endl;
        } else {
            escreve << "deles" << std::endl;
        }
        escreve.close();
    }
    
};

#endif	/* VINICIUSTECNICO_H */

