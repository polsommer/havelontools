package script.theme_park.dungeon.mustafar_trials.mining_facility;

import script.library.sequencer;
import script.obj_id;

public class drunk2 extends script.base_script
{
    public drunk2()
    {
    }
    public int OnAttach(obj_id self) throws InterruptedException
    {
        sequencer.registerSequenceObject(self, "drunk_go2");
        return SCRIPT_CONTINUE;
    }
}
