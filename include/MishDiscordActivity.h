/*
It is called "MishDiscordActivity" because DiscordActivity is already defined in discord sdk itself
and I don't want to introduce an extra namespace solely for this thing.
*/

#ifndef MishDiscordActivity_h_
#define MishDiscordActivity_h_

#include <string>
#include "../discord/discord.h"
#define DRPC_APP_ID 1225013717247328358

class MishDiscordActivity {
public:
    MishDiscordActivity();
    ~MishDiscordActivity();

    void setActivity(const discord::Activity activity);
    void clearActivity();

    int init();
    void update();
    void cleanup();
private:
    discord::Core* core{ nullptr };
};

#endif