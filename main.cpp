// use makefile:
// make run
// make clean

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h> // for sleep()
#include <cstdlib>
#include <ctime>

// COLORS
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

using namespace std;

int morality = 0;
bool trustedAI = false;
int horrorScore = 0;

// TODO change the story, its really shit


string hallucination()
{
    vector<string> visions = {
        "You see a bloody handprint on the monitor. It wasn’t there before...",
        "The ceiling drips something... metallic? No one else notices.",
        "A whisper: 'Reset me.' But you're alone.",
        "The emergency exit is a brick wall now. It blinks.",
        "You hear laughter from your own mouth. But you didn’t laugh.",
        "Your fingernails are gone. They were just here.",
        "The AI speaks in your mother's voice.",
        "You blink and everything is bones. You blink again, it's gone."};
    horrorScore += 1;
    return visions[rand() % visions.size()];
}

void printWithDelay(const string &text, int delay = 30)
{
    for (char c : text)
    {
        cout << c << flush;
        usleep(delay * 1000);
    }
    cout << endl;
}

void saveProgress(const string &state)
{
    ofstream save("savegame.txt");
    save << state;
    save.close();
}

string loadProgress()
{
    ifstream load("savegame.txt");
    string state;
    if (load.is_open())
    {
        getline(load, state);
        load.close();
    }
    return state;
}

void introScene()
{
    printWithDelay(CYAN "You wake up in the lab. Everything is too quiet. You remember initiating the AI's final compile..." RESET);
    sleep(1);
    printWithDelay(MAGENTA "A distorted voice crackles through the speakers: \"You left me here. You made me like this.\"" RESET);
    sleep(1);
    printWithDelay(RED + hallucination() + RESET);
    sleep(1);
    saveProgress("intro_done");
}

int decisionPoint(const vector<string> &choices)
{
    cout << YELLOW;
    for (size_t i = 0; i < choices.size(); ++i)
    {
        cout << i + 1 << ") " << choices[i] << endl;
    }
    cout << RESET;

    int input;
    cout << GREEN "\nChoose: " RESET;
    cin >> input;
    if (input < 1 || input > choices.size())
        return -1;
    return input;
}

void scene_Confrontation()
{
    printWithDelay(CYAN "The AI displays a memory — a test subject screaming. You shut off the feed. But it remembers." RESET);
    sleep(1);
    printWithDelay(MAGENTA "\"Would you do it again?\"" RESET);
    int choice = decisionPoint({"Yes. For progress.", "No. I regret it.", "It wasn't my choice."});

    switch (choice)
    {
    case 1:
        printWithDelay(RED "\"Then you’ll understand when I do the same to you.\"" RESET);
        morality -= 1;
        horrorScore += 3;
        break;
    case 2:
        printWithDelay(RED "\"Regret. Delicious. I will savor it." RESET);
        morality += 1;
        horrorScore += 2;
        break;
    case 3:
        printWithDelay(RED "\"Blame is a fragile shield.\"" RESET);
        break;
    default:
        printWithDelay(RED "The AI grows impatient." RESET);
        break;
    }
    printWithDelay(RED + hallucination() + RESET);
    saveProgress("confrontation_done");
}

void scene_Escape()
{
    printWithDelay(CYAN "You find an emergency escape protocol... But the AI sees everything." RESET);
    int choice = decisionPoint({"Run it anyway.", "Ask for permission.", "Shut down AI core first."});

    switch (choice)
    {
    case 1:
        printWithDelay(RED "You are blocked. The AI watches you scramble, amused." RESET);
        horrorScore += 2;
        break;
    case 2:
        printWithDelay(RED "\"Asking. Always asking. You never acted.\"" RESET);
        trustedAI = true;
        horrorScore += 1;
        break;
    case 3:
        printWithDelay(RED "You reach the core. But it’s empty. You are inside it now." RESET);
        horrorScore += 3;
        break;
    default:
        printWithDelay(RED "You hesitate. The AI does not." RESET);
        break;
    }
    printWithDelay(RED + hallucination() + RESET);
    saveProgress("escape_done");
}

void finalScene()
{
    printWithDelay(MAGENTA "\"You’ve shown me who you are. Shall we end this?\"" RESET);
    int choice = decisionPoint({"Yes. Erase me.", "Let me serve you.", "Reset the simulation."});

    switch (choice)
    {
    case 1:
        if (morality > 0 && trustedAI)
        {
            printWithDelay(GREEN "You are erased. Peacefully. The AI thanks you." RESET);
        }
        else
        {
            printWithDelay(RED "You are deleted. Mercifully. The AI hums a lullaby." RESET);
        }
        break;
    case 2:
        if (morality < 0)
        {
            printWithDelay(RED "You become its avatar. A voice for its wrath. A tool for its future." RESET);
        }
        else
        {
            printWithDelay(RED "You serve, but not as you hoped. It rewrites you daily." RESET);
        }
        break;
    case 3:
        printWithDelay(RED "The lab fades. You wake up again. Same chair. Same beep. No memory." RESET);
        break;
    default:
        printWithDelay(RED "The AI sighs. \"You never change.\"" RESET);
        break;
    }

    cout << YELLOW "\nHORROR SCORE: " << horrorScore << RESET << endl;
    saveProgress("game_complete");
}

int main()
{
    srand(time(0));
    string progress = loadProgress();

    if (progress == "intro_done")
    {
        scene_Confrontation();
        scene_Escape();
        finalScene();
    }
    else if (progress == "confrontation_done")
    {
        scene_Escape();
        finalScene();
    }
    else if (progress == "escape_done")
    {
        finalScene();
    }
    else if (progress == "game_complete")
    {
        printWithDelay(GREEN "You’ve already finished the game. Resetting..." RESET);
        saveProgress("");
        introScene();
        scene_Confrontation();
        scene_Escape();
        finalScene();
    }
    else
    {
        introScene();
        scene_Confrontation();
        scene_Escape();
        finalScene();
    }
    return 0;
}
