#include "../include/LevelLoader.h"
#include "../include/CI72Locker.h"

LevelLoader::LevelLoader() : state(0) {}
LevelLoader::~LevelLoader() {}

const char* testtextline =
"DUE to your greed and selfishness people are now arguing with whoever they can, what I can tell you right now is this:"
" I will do anything to get an unlocked version of IAMT I am even capable of hacking someone's computer to do it even yours"
" so if you care about data security then please release the damn program, it's already unbearable, what do you think you are?"
" I advise you not to even try anything\n";

const char* textlines_intro[] = {
    "hmm...",
    "this doesn't look very IAMT",
    "Where are the buttons?",
    "Where are the files if they are exist?",
    "...",
    "ok how do I leave"
};

const char* dickclark_text = "Oh my god it;s DICK CLARK somehting somethin";

int intro_text_delay_cnt = 0;
int intro_text_current = 0;

bool canChangeLevel = true;

void LevelLoader::update() {
    //fprintf_s(stdout, "LevelLoader::update(), state = %d\n", state);
    switch (state) {
    case State::INTRO: {
        ++intro_text_delay_cnt;
        if (intro_text_delay_cnt > 60 * 4) { ++intro_text_current; intro_text_delay_cnt = 0; }
        if (intro_text_current == 6) { changeState(State::TITLE); }
        if (game->input_state.enter) {
            intro_text_current = 6;
        }
        player.update();
        break;
    }
    case State::TITLE: {
        if (!(game->input_state.mouseHeld || game->input_state.enter)) {
            canChangeLevel = true;
        }
        if ((game->input_state.mouseHeld || game->input_state.enter) && canChangeLevel) {
            game->input_state.enter = false;
            changeState(State::LVL_CHICKENS);
        }
        break;
    }
    case State::LVL_CHICKENS: {
        player.update();
        if (!(player.canShoot || game->input_state.mouseHeld)) {
            player.canShoot = true;
        }
        game->bulletSpawner.update();
        game->chicken_handler.update();
        if (game->input_state.dbg_reload) {
            game->input_state.dbg_reload = false;
            changeState(State::LVL_CHICKENS);
        }
        if (game->input_state.enter && player.isDead) {
            changeState(State::TITLE);
        }
        if (game->chicken_handler.level_finished && !(player.isDead)) {
            changeState(State::LVL_DICK_CLARK);
        }
        break;
    }
    case State::LVL_DICK_CLARK: {
        player.update();
        if (!(player.canShoot || game->input_state.mouseHeld)) {
            player.canShoot = true;
        }
        game->bulletSpawner.update();
        game->dickclark_handler.update();
        if (game->input_state.dbg_reload) {
            game->input_state.dbg_reload = false;
            changeState(State::LVL_DICK_CLARK);
        }
        if (game->input_state.enter && player.isDead) {
            changeState(State::TITLE);
        }
        if (game->dickclark_handler.level_finished && !(player.isDead)) {
            changeState(State::LVL_USELESS);
        }
        break;
    }
    case State::LVL_USELESS:
        break;
    default:
        break;
    }
}

void LevelLoader::render() {
    //fprintf_s(stdout, "LevelLoader::render(), state = %d\n", state);
    switch (state) {
    case State::INTRO: {
        if (intro_text_current < 6) {
            game->drawText(
                game->font,
                textlines_intro[intro_text_current],
                { 20, 25, game->getWindowDimensions().w - 20, game->getWindowDimensions().h - 25 });
        }
        player.render();
        break;
    }
    case State::TITLE: {
        game->drawText(game->font_impact, "CHICKEN INVADERS\n72: IAMT 3.5.2: GUI:\nNO", { game->getWindowDimensions().w / 5, game->getWindowDimensions().h / 5, game->getWindowDimensions().w / 4 * 3 });
        game->drawText(game->font, "Happy april fools, fools\ngood luck closing this lmao. Now I am hte virus.\n\n(psst.. )you can press left mouse button to go to the next level\n\n\n\n\noh and you can change\nvolume with + and - if it's too loud", { game->getWindowDimensions().w / 5, game->getWindowDimensions().h / 3 * 2 });
        break;
    }
    case State::LVL_CHICKENS: {
        player.render();
        game->bulletSpawner.render();
        game->chicken_handler.render();
        game->drawText(game->font, testtextline, { 20, 25, game->getWindowDimensions().w - 20, game->getWindowDimensions().h - 25 });

        if (player.isDead) {
            game->drawText(game->font_impact, "sir you have failed miserably", { game->getWindowDimensions().w / 5, game->getWindowDimensions().w / 5, game->getWindowDimensions().w / 2, game->getWindowDimensions().h / 3 });
        }
        break;
    }
    case State::LVL_DICK_CLARK:
        player.render();
        game->bulletSpawner.render();
        game->dickclark_handler.render();

        game->drawText(game->font, testtextline, { 20, 25, game->getWindowDimensions().w - 20, game->getWindowDimensions().h - 25 });
        if (player.isDead) {
            game->drawText(game->font_impact, "sir you have failed miserably", { game->getWindowDimensions().w / 5, game->getWindowDimensions().w / 5, game->getWindowDimensions().w / 2, game->getWindowDimensions().h / 3 });
        }
        break;
    case State::LVL_USELESS:
        break;
    default:
        break;
    }
}

void LevelLoader::loadState(int state) {
    //fprintf_s(stdout, "loadState(%d)\n", state);
    this->state = state;
    switch (state) {
    case State::INTRO:
        player.canShoot = false;
        //TODO: Add intro music?
        break;
    case State::TITLE:
        if (game->input_state.mouseHeld) {
            canChangeLevel = false;
        }
        break;
    case State::LVL_CHICKENS: {
        SDL_WarpMouseInWindow(game->getWindow(), game->getWindowDimensions().w / 2, game->getWindowDimensions().h - 50);
        if (game->input_state.mouseHeld) {
            player.canShoot = false;
        }
        SDL_FPoint chicken_positions[MAX_CHICKENS] = {};
        for (int i = 0; i < MAX_CHICKENS; ++i) {
            SDL_FPoint new_pos = {
                float(rand() % (game->getWindowDimensions().w - 50)),
                float(rand() % game->getWindowDimensions().h / 2) };
            game->chicken_handler.spawn(new_pos, 3);
        }
        game->chicken_handler.shoot_cooldown = 200;
        break;
    }
    case State::LVL_DICK_CLARK: {
        SDL_WarpMouseInWindow(game->getWindow(), game->getWindowDimensions().w / 2, game->getWindowDimensions().h - 50);
        if (game->input_state.mouseHeld) {
            player.canShoot = false;
        }
        SDL_FPoint dickclark_positions[MAX_DICKCLARKS] = {};
        for (int i = 0; i < MAX_DICKCLARKS; ++i) {
            SDL_FPoint new_pos = {
                float(rand() % (game->getWindowDimensions().w - 50)),
                float(rand() % game->getWindowDimensions().h / 2) };
            game->dickclark_handler.spawn(new_pos, 5);
        }
        game->dickclark_handler.shoot_cooldown = 200;
        break;
    }
    case State::LVL_USELESS:
        break;
    default:
        break;
    }
}

void LevelLoader::unloadState() {
    //fprintf_s(stdout, "unloadState()\n");
    switch (state) {
    case State::INTRO:
        intro_text_current = 0;
        intro_text_delay_cnt = 0;
        player.canShoot = true;
        canChangeLevel = false;
        break;
    case State::TITLE:
        break;
    case State::LVL_CHICKENS:
        game->chicken_handler.cleanup();
        game->bulletSpawner.cleanup();
        player.isDead = false;
        canChangeLevel = false;
        break;
    case State::LVL_DICK_CLARK:
        game->dickclark_handler.cleanup();
        game->bulletSpawner.cleanup();
        player.isDead = false;
        canChangeLevel = false;
        break;
    case State::LVL_USELESS:
        break;
    default:
        break;
    }
}

void LevelLoader::changeState(int state) {
    fprintf_s(stdout, "changeState(%d)\n", state);
    unloadState();
    loadState(state);
}

SDL_FPoint LevelLoader::getPlayerPosition() {
    return player.getPosition();
}