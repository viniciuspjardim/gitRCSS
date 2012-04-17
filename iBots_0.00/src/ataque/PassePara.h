/*
 * File:   PassePara.h
 * Author: vinicius
 *
 * Created on April 17, 2012, 3:49 PM
 */

#ifndef PASSEPARA_H
#define	PASSEPARA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bhv_pass_kick_find_receiver.h"

#include "action_chain_holder.h"
#include "action_chain_graph.h"
#include "field_analyzer.h"

#include "neck_turn_to_receiver.h"

#include <rcsc/action/bhv_scan_field.h>
#include <rcsc/action/body_hold_ball.h>
#include <rcsc/action/body_kick_one_step.h>
#include <rcsc/action/body_smart_kick.h>
#include <rcsc/action/body_stop_ball.h>
#include <rcsc/action/body_turn_to_point.h>
#include <rcsc/action/neck_scan_field.h>
#include <rcsc/action/neck_turn_to_point.h>
#include <rcsc/action/neck_turn_to_player_or_scan.h>
#include <rcsc/action/view_synch.h>
#include <rcsc/action/kick_table.h>
#include <rcsc/player/player_agent.h>
#include <rcsc/player/abstract_player_object.h>
#include <rcsc/player/soccer_intention.h>
#include <rcsc/player/intercept_table.h>
#include <rcsc/player/say_message_builder.h>
#include <rcsc/common/server_param.h>
#include <rcsc/common/logger.h>
#include <rcsc/math_util.h>

#include <vector>
#include <algorithm>
#include <cmath>

using namespace rcsc;

class PassePara
: public SoccerIntention {
private:
    int M_step;
    int M_receiver_unum;
    Vector2D M_receive_point;

public:

    PassePara(const int receiver_unum,
            const Vector2D & receive_point)
    : M_step(0),
    M_receiver_unum(receiver_unum),
    M_receive_point(receive_point) {
    }

    bool finished(const PlayerAgent * agent) {
        ++M_step;

        dlog.addText(Logger::TEAM,
                __FILE__": (finished) step=%d",
                M_step);

        if (M_step >= 2) {
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) time over");
            return true;
        }

        const WorldModel & wm = agent->world();

        //
        // check kickable
        //

        if (!wm.self().isKickable()) {
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) no kickable");
            return true;
        }

        //
        // check receiver
        //

        const AbstractPlayerObject * receiver = wm.ourPlayer(M_receiver_unum);

        if (!receiver) {
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) NULL receiver");
            return true;
        }

        if (receiver->unum() == wm.self().unum()) {
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) receiver is self");
            return true;
        }

        if (receiver->posCount() <= 0) {
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) already seen");
            return true;
        }

        //
        // check opponent
        //

        if (wm.existKickableOpponent()) {
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) exist kickable opponent");
            return true;
        }

        if (wm.interceptTable()->opponentReachCycle() <= 1) {
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) opponent may be kickable");
            return true;
        }

        //
        // check next kickable
        //

        double kickable2 = std::pow(wm.self().playerType().kickableArea()
                - wm.self().vel().r() * ServerParam::i().playerRand()
                - wm.ball().vel().r() * ServerParam::i().ballRand()
                - 0.15,
                2);
        Vector2D self_next = wm.self().pos() + wm.self().vel();
        Vector2D ball_next = wm.ball().pos() + wm.ball().vel();

        if (self_next.dist2(ball_next) > kickable2) {
            // unkickable if turn is performed.
            dlog.addText(Logger::TEAM,
                    __FILE__": (finished) unkickable at next cycle");
            return true;
        }

        return false;
    }

    bool execute(PlayerAgent * agent) {
        const WorldModel & wm = agent->world();
        const AbstractPlayerObject * receiver = wm.ourPlayer(M_receiver_unum);

        if (!receiver) {
            return false;
        }

        const Vector2D next_self_pos = agent->effector().queuedNextMyPos();
        const AngleDeg next_self_body = agent->effector().queuedNextMyBody();
        const double next_view_width = agent->effector().queuedNextViewWidth().width() * 0.5;

        const Vector2D receiver_pos = receiver->pos() + receiver->vel();
        const AngleDeg receiver_angle = (receiver_pos - next_self_pos).th();

        Vector2D face_point = (receiver_pos + M_receive_point) * 0.5;
        AngleDeg face_angle = (face_point - next_self_pos).th();

        double rate = 0.5;
        while ((face_angle - receiver_angle).abs() > next_view_width - 10.0) {
            rate += 0.1;
            face_point
                    = receiver_pos * rate
                    + M_receive_point * (1.0 - rate);
            face_angle = (face_point - next_self_pos).th();

            if (rate > 0.999) {
                break;
            }
        }

        dlog.addText(Logger::TEAM,
                __FILE__": (intentin) facePoint=(%.1f %.1f) faceAngle=%.1f",
                face_point.x, face_point.y,
                face_angle.degree());
        agent->debugClient().addMessage("IntentionTurnToReceiver%.0f",
                face_angle.degree());
        Body_TurnToPoint(face_point).execute(agent);
        agent->setNeckAction(new Neck_TurnToPoint(face_point));

        return true;
    }

private:

};


#endif	/* PASSEPARA_H */

