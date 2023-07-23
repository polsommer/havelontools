package script.swgplus_scripts;

import script.obj_id;

public class broadcasting_script extends script.base_script
{
    public broadcasting_script()
    {
    }
    public int OnHearSpeech(obj_id self, obj_id objSpeaker, String strText) throws InterruptedException
    {
        if (strText.equals("broadcast"))
        {
            sendSystemMessageGalaxyTestingOnly("Broadcasting now, speaker is "+ self);
        }
        return SCRIPT_CONTINUE;
    }
}
