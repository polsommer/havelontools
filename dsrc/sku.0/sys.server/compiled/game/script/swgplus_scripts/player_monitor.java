package script.swgplus_scripts;

import script.*;
import script.library.*;

public class player_monitor extends script.base_script {
    boolean debug = false;
    int cnt = 0;

    public int OnAttach(obj_id self) throws InterruptedException {
        if (getMovementPercent(self) > 300 && !isGod(self)) {
            debugSpeakMsg(self, "Speed is too fast!");
            //disconnectPlayer(self);
            //Need number
        }
        setObjVar(self, "player.monitor_x", getLocation(self).x);
        setObjVar(self, "player.monitor_y", getLocation(self).y);
        setObjVar(self, "player.monitor_z", getLocation(self).z);
        setObjVar(self, "player.monitor_scene", getLocation(self).area);
        return SCRIPT_CONTINUE;
    }
    public int OnInitialize(obj_id self) throws InterruptedException{
        debugSpeakMsg(self, "Speed percent is: " + getMovementPercent(self));
        debugSpeakMsg(self, "Speed is: " + getMovementSpeed(self));
        return SCRIPT_CONTINUE;
    }

    public int OnLogout(obj_id self) throws InterruptedException{
        removeObjVar(self, "player.monitor_x");
        removeObjVar(self, "player.monitor_y");
        removeObjVar(self, "player.monitor_z");
        removeObjVar(self, "player.monitor_scene");


        return SCRIPT_CONTINUE;
    }
    public int OnMoveMoving(obj_id self) throws InterruptedException{
        debugSpeakMsg(self, "Speed percent is: " + getMovementPercent(self));
        debugSpeakMsg(self, "Speed is: " + getMovementSpeed(self));
        return SCRIPT_CONTINUE;
    }
    public int OnLocomotionChanged(obj_id self, int newLocomotion, int oldLocomotion) throws InterruptedException{
        speedCheck(self);
        return SCRIPT_CONTINUE;
    }

    public int OnSpeaking(obj_id self, String text) throws InterruptedException {
        if (text.equals("st") && isGod(self)) {
            debugSpeakMsg(self, "Speed percent is: " + getMovementPercent(self));
            debugSpeakMsg(self, "Speed is: " + getMovementSpeed(self));
        }
        if (text.equals("st_debugtrue") && isGod(self)) {
            debug = true;
        }
        if (text.equals("st_debugfalse") && isGod(self)) {
            debug = false;
        }
        if (text.equals("st_dst") && isGod(self)) {
            {
                float distance = getDistance(self,
                        new location(
                                getFloatObjVar(self, "player.monitor_x"),
                                getFloatObjVar(self, "player.monitor_y"),
                                getFloatObjVar(self, "player.monitor_z"),
                                getStringObjVar(self, "player.monitor_scene")));
                debugSpeakMsg(self, "Distance between last point is: " + distance);

                setObjVar(self, "player.monitor_x", getLocation(self).x);
                setObjVar(self, "player.monitor_y", getLocation(self).y);
                setObjVar(self, "player.monitor_z", getLocation(self).z);
                setObjVar(self, "player.monitor_scene", getLocation(self).area);
            }
        }
        return SCRIPT_CONTINUE;
    }

    public int speedCheck(obj_id self){
        if (getMovementPercent(self) > 3.5f && !debug) {
            debugSpeakMsg(self, "Speed is too fast!");
            //disconnectPlayer(self);
            return  -1;
        }
        return 0;
    }
}
