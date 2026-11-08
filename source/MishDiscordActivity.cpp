#include "../include/MishDiscordActivity.h"

MishDiscordActivity::MishDiscordActivity() {

}

MishDiscordActivity::~MishDiscordActivity() {
    if (core) { core->~Core(); }
}


void MishDiscordActivity::setActivity(const discord::Activity activity) {
    if (core) {
        core->ActivityManager().UpdateActivity(activity, nullptr);
    }
}

void MishDiscordActivity::clearActivity() {
    // I don't really care about the return value because it won't change anything
    if (core) {
        core->ActivityManager().ClearActivity(nullptr);
    }
}


int MishDiscordActivity::init() {
    discord::Core::Create(DRPC_APP_ID, DiscordCreateFlags_NoRequireDiscord, &core);
    return 0;
}

void MishDiscordActivity::update() {
    if (core) {
        core->RunCallbacks();
    }
}

void MishDiscordActivity::cleanup() {
    if (core) {
        core->~Core();
    }
}