package script.beta;

import script.*;
import script.library.*;

import java.util.HashSet;

import static script.library.holiday.closeOldWindow;

public class pilot_test extends base_script
{
    public pilot_test(){}
    public static final String HANDLER_SET_TOOL_CLASS = "handleSetToolClass";
    public static final string_id REDEEM= new string_id("sui", "redeem_pilot_token");
    public static final String[] SETS = {
            "Master Freelance Pilot",
            "Master Imperial Pilot",
            "Master Rebel Pilot"
    };

    public int OnObjectMenuRequest(obj_id self, obj_id player, menu_info mi) throws InterruptedException
    {
        int menu = mi.addRootMenu(menu_info_types.ITEM_USE, REDEEM);

        return SCRIPT_CONTINUE;
    }
    public int OnObjectMenuSelect(obj_id self, obj_id player, int item) throws InterruptedException
    {
        startSetSelection(player);
        return SCRIPT_CONTINUE;
    }
    public void startSetSelection(obj_id player) throws InterruptedException
    {
        obj_id self = getSelf();
        String prompt = "Choose a Roadmap:";
        String title = "Master Pilot Skill Token";
        int pid = sui.listbox(self, player, prompt, sui.OK_CANCEL, title, SETS, "handleOptionSelect", true, false);
        setWindowPid(player, pid);
    }
    public void setWindowPid(obj_id player, int pid) throws InterruptedException
    {
        if (pid > -1)
        {
            utils.setScriptVar(player, "character_builder.pid", pid);
        }
    }
    public int OnInitialize(obj_id self) throws InterruptedException
    {
        setObjVar(self, "noTradeShared", true);
        return SCRIPT_CONTINUE;
    }
    public int handleOptionSelect(obj_id self, dictionary params) throws InterruptedException {
        if (params == null || params.isEmpty()) {
            return SCRIPT_CONTINUE;
        }
        obj_id player = sui.getPlayerId(params);
        int btn = sui.getIntButtonPressed(params);
        int idx = sui.getListboxSelectedRow(params);
        if (btn == sui.BP_CANCEL) {
            cleanScriptVars(player);
            return SCRIPT_CONTINUE;
        }
        if (idx == -1 || idx > SETS.length) {
            cleanScriptVars(player);
            return SCRIPT_CONTINUE;
        }
       // sendSystemMessageTestingOnly(player, "Index Selected: "+idx);
        grantSkill(player,idx,self);
        closeOldWindow(player);
        return SCRIPT_CONTINUE;
    }
    public void cleanScriptVars(obj_id player) throws InterruptedException
    {
        obj_id self = getSelf();
        utils.removeScriptVarTree(player, "character_builder");
        utils.removeScriptVarTree(self, "character_builder");
    }
    public static boolean revokeSpaceSkills(obj_id player) throws InterruptedException
    {
        if (hasSkill(player, "pilot_rebel_navy_novice") || hasSkill(player, "pilot_imperial_navy_novice") || hasSkill(player, "pilot_neutral_novice"))
        {
            String pilotFaction = "";
            if (!utils.hasScriptVar(player, "revokePilotSkill"))
            {
                utils.setScriptVar(player, "revokePilotSkill", 1);
            }
            if (hasSkill(player, "pilot_rebel_navy_novice"))
            {
                pilotFaction = "rebel_navy";
            }
            else if (hasSkill(player, "pilot_imperial_navy_novice"))
            {
                pilotFaction = "imperial_navy";
            }
            else if (hasSkill(player, "pilot_neutral_novice"))
            {
                pilotFaction = "neutral";
            }
            if (!pilotFaction.equals(""))
            {
                for (int i = 0; i < space_skill.SKILL_NAMES.length; i++)
                {
                    skill.revokeSkill(player, "pilot_" + pilotFaction + space_skill.SKILL_NAMES[i]);
                }
                utils.removeScriptVar(player, "revokePilotSkill");
                sendSystemMessageTestingOnly(player, "Pilot Skills Revoked.");
                return true;
            }
        }
        return false;
    }
    public static obj_id[] grantSkill(obj_id player, int num, obj_id destroy_me) throws InterruptedException
    {
        boolean pilotRevoked = true;
        if (space_skill.hasSpaceSkills(player))
        {
            pilotRevoked = revokeSpaceSkills(player);
        }
        if (!pilotRevoked)
        {
            sendSystemMessageTestingOnly(player, "The system was unable to revoke your pilot skills.");
            return null;
        }
        if (num == 0){
            skill.grantSkill(player, "pilot_neutral_novice");
            skill.grantSkill(player, "pilot_neutral_starships_01");
            skill.grantSkill(player, "pilot_neutral_starships_02");
            skill.grantSkill(player, "pilot_neutral_starships_03");
            skill.grantSkill(player, "pilot_neutral_starships_04");
            skill.grantSkill(player, "pilot_neutral_weapons_01");
            skill.grantSkill(player, "pilot_neutral_weapons_02");
            skill.grantSkill(player, "pilot_neutral_weapons_03");
            skill.grantSkill(player, "pilot_neutral_weapons_04");
            skill.grantSkill(player, "pilot_neutral_procedures_01");
            skill.grantSkill(player, "pilot_neutral_procedures_02");
            skill.grantSkill(player, "pilot_neutral_procedures_03");
            skill.grantSkill(player, "pilot_neutral_procedures_04");
            skill.grantSkill(player, "pilot_neutral_droid_01");
            skill.grantSkill(player, "pilot_neutral_droid_02");
            skill.grantSkill(player, "pilot_neutral_droid_03");
            skill.grantSkill(player, "pilot_neutral_droid_04");
            skill.grantSkill(player, "pilot_neutral_master");
            sendSystemMessageTestingOnly(player, "Master Privateer Pilot skills received.");
        }
        if (num == 1){
            skill.grantSkill(player, "pilot_imperial_navy_novice");
            skill.grantSkill(player, "pilot_imperial_navy_starships_01");
            skill.grantSkill(player, "pilot_imperial_navy_starships_02");
            skill.grantSkill(player, "pilot_imperial_navy_starships_03");
            skill.grantSkill(player, "pilot_imperial_navy_starships_04");
            skill.grantSkill(player, "pilot_imperial_navy_weapons_01");
            skill.grantSkill(player, "pilot_imperial_navy_weapons_02");
            skill.grantSkill(player, "pilot_imperial_navy_weapons_03");
            skill.grantSkill(player, "pilot_imperial_navy_weapons_04");
            skill.grantSkill(player, "pilot_imperial_navy_procedures_01");
            skill.grantSkill(player, "pilot_imperial_navy_procedures_02");
            skill.grantSkill(player, "pilot_imperial_navy_procedures_03");
            skill.grantSkill(player, "pilot_imperial_navy_procedures_04");
            skill.grantSkill(player, "pilot_imperial_navy_droid_01");
            skill.grantSkill(player, "pilot_imperial_navy_droid_02");
            skill.grantSkill(player, "pilot_imperial_navy_droid_03");
            skill.grantSkill(player, "pilot_imperial_navy_droid_04");
            skill.grantSkill(player, "pilot_imperial_navy_master");
            sendSystemMessageTestingOnly(player, "Master Imperial Pilot skills received.");
        }
        if (num == 2){
            skill.grantSkill(player, "pilot_rebel_navy_novice");
            skill.grantSkill(player, "pilot_rebel_navy_starships_01");
            skill.grantSkill(player, "pilot_rebel_navy_starships_02");
            skill.grantSkill(player, "pilot_rebel_navy_starships_03");
            skill.grantSkill(player, "pilot_rebel_navy_starships_04");
            skill.grantSkill(player, "pilot_rebel_navy_weapons_01");
            skill.grantSkill(player, "pilot_rebel_navy_weapons_02");
            skill.grantSkill(player, "pilot_rebel_navy_weapons_03");
            skill.grantSkill(player, "pilot_rebel_navy_weapons_04");
            skill.grantSkill(player, "pilot_rebel_navy_procedures_01");
            skill.grantSkill(player, "pilot_rebel_navy_procedures_02");
            skill.grantSkill(player, "pilot_rebel_navy_procedures_03");
            skill.grantSkill(player, "pilot_rebel_navy_procedures_04");
            skill.grantSkill(player, "pilot_rebel_navy_droid_01");
            skill.grantSkill(player, "pilot_rebel_navy_droid_02");
            skill.grantSkill(player, "pilot_rebel_navy_droid_03");
            skill.grantSkill(player, "pilot_rebel_navy_droid_04");
            skill.grantSkill(player, "pilot_rebel_navy_master");
            sendSystemMessageTestingOnly(player, "Master Rebel Pilot skills received.");
        }
        if (num>2){
            sendSystemMessageTestingOnly(player, "Index Selected: "+num+", Is out of bounds. Error. Try again.");
            return null;
        }

        static_item.destroyObject(destroy_me);
        return null;
    }

}
