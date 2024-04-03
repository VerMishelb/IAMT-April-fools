#include "../include/MishDiscordActivity.h"

MishDiscordActivity::MishDiscordActivity() {

}

MishDiscordActivity::~MishDiscordActivity() {
    core->~Core();
}


void MishDiscordActivity::setActivity(const discord::Activity activity) {
    core->ActivityManager().UpdateActivity(activity, nullptr);
}

void MishDiscordActivity::clearActivity() {
    // I don't really care about the return value because it won't change anything
    core->ActivityManager().ClearActivity(nullptr);
}


int MishDiscordActivity::init() {
    discord::Core::Create(DRPC_APP_ID, DiscordCreateFlags_NoRequireDiscord, &core);
    return 0;
}

void MishDiscordActivity::update() {
    core->RunCallbacks();
}

void MishDiscordActivity::cleanup() {
    core->~Core();
}