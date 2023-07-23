package script.npc.converse;

import script.dictionary;
import script.library.*;
import script.obj_id;

public class junk_dealer extends script.base_script
{
    public junk_dealer()
    {
    }
    public static final boolean LOGGING_ON = true;
    public static final String LOGNAME = "junk_log";
    public int OnAttach(obj_id self) throws InterruptedException
    {
        ai_lib.setDefaultCalmBehavior(self, ai_lib.BEHAVIOR_SENTINEL);
        String creatureName = getCreatureName(self);
        if (creatureName.equals("junk_jawa"))
        {
            return SCRIPT_CONTINUE;
        }
        if (!creatureName.equals("junk_dealer_smuggler"))
        {
            attachScript(self, "conversation.junk_dealer_generic");
        }
        else 
        {
            attachScript(self, "conversation.junk_dealer_smuggler");
        }
        return SCRIPT_CONTINUE;
    }
    public int startDealing(obj_id self, dictionary params) throws InterruptedException
    {
        obj_id player = params.getObjId("player");
        if (utils.outOfRange(self, player, 10.0f, true))
        {
            return SCRIPT_CONTINUE;
        }
        smuggler.showSellJunkSui(player, self, false, false);
        return SCRIPT_CONTINUE;
    }
    public int handleSellJunkSui(obj_id self, dictionary params) throws InterruptedException
    {
        obj_id player = sui.getPlayerId(params);
        if (!isIdValid(player))
        {
            return SCRIPT_CONTINUE;
        }
        if (utils.outOfRange(self, player, 10.0f, true))
        {
            return SCRIPT_CONTINUE;
        }
        utils.setScriptVar(player, "fence", false);
        utils.setScriptVar(player, "junk_dealer_transaction", self);
        messageTo(player, "handleSellJunkSui", params, 0.0f, false);
        return SCRIPT_CONTINUE;
    }
    public int handleSoldJunk(obj_id self, dictionary params) throws InterruptedException
    {
        if (params == null || params.isEmpty())
        {
            return SCRIPT_CONTINUE;
        }
        obj_id player = params.getObjId(money.DICT_TARGET_ID);
        if (!isIdValid(player))
        {
            return SCRIPT_CONTINUE;
        }
        if (utils.outOfRange(self, player, 10.0f, true))
        {
            return SCRIPT_CONTINUE;
        }
        blog("junk_dealer.handleSoldJunk() - setting fence to false");
        params.put("fence", false);
        messageTo(player, "handleSoldJunk", params, 0.0f, false);
        return SCRIPT_CONTINUE;
    }
    public int startFlaggingItemsNoSale(obj_id self, dictionary params) throws InterruptedException
    {
        obj_id player = params.getObjId("player");
        if (utils.outOfRange(self, player, 10.0f, true))
        {
            return SCRIPT_CONTINUE;
        }
        smuggler.flagJunkSaleSui(player, self);
        return SCRIPT_CONTINUE;
    }
    public int handleFlagJunkSui(obj_id self, dictionary params) throws InterruptedException
    {
        obj_id player = sui.getPlayerId(params);
        if (!isIdValid(player))
        {
            return SCRIPT_CONTINUE;
        }
        if (utils.outOfRange(self, player, 10.0f, true))
        {
            return SCRIPT_CONTINUE;
        }
        blog("junk_dealer.handleFlagJunkSui() - sending params to player.");
        params.put("dealer", self);
        messageTo(player, "handleFlagJunkSui", params, 0.0f, false);
        return SCRIPT_CONTINUE;
    }
    public boolean blog(String txt) throws InterruptedException
    {
        if (LOGGING_ON)
        {
            LOG(LOGNAME, txt);
        }
        return true;
    }
}
