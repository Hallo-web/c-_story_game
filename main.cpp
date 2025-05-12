// use makefile:
// make run
// make clean

#include <iostream> // for basic funtionality
#include <vector> // for vectors
#include <string> // for strings
#include <fstream> // no fucking clue
#include <unistd.h> // for sleep()
#include <cstdlib> // no fucking clue
#include <ctime> // for the timings, delay and so on
#include <oneapi/tbb/task_arena.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

using namespace std;

struct Player {
    string username;
    string password;
    int age;
    int strength;
    int intelligence;
    int dexterity;
};

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

// lets make this like a unix/linux bootup splashscreen
void firstOSIRISbootup() {
    printWithDelay(GREEN "[OK]" RESET " Mounting user partition");
    printWithDelay(GREEN "[OK]" RESET " Connecting to local node DC-07");
    printWithDelay(GREEN "[OK]" RESET " Validating personnel profile");
    printWithDelay(GREEN "[OK]" RESET " Initializing OSIRIS core bindings");
    sleep(1);
    printWithDelay(RED "[WARNING]" RESET " Cognitive sync drift detected (Δ0.37ms)");
    sleep(1);
    printWithDelay(GREEN "[OK]" RESET " Booting up OSIRIS...\n");
    sleep(2);
    printWithDelay(GREEN "==== O.S.I.R.I.S ====" RESET);
    printf("\n\n");
}

void PlayerStats(Player& player)
{
    cout << CYAN "\n--- Player Stats ---" RESET << endl;
    cout << "Name: " << player.username << endl;
    cout << "Age: " << player.age << endl;
    cout << "Strength: " << player.strength << endl;
    cout << "Intelligence: " << player.intelligence << endl;
    cout << "Dexterity: " << player.dexterity << endl;
    cout << endl;
}


Player firstloginScreen()
{
    Player player;
    sleep(1);

    printWithDelay("Set a new Username:");
    cout << ">> ";
    cin >> player.username;

    printWithDelay("Set a new Password:");
    cout << ">> ";
    cin >> player.password;

    printWithDelay("Set your Age:");
    cout << ">> ";
    cin >> player.age;

    printWithDelay("Set your Strength:");
    cout << ">> ";
    cin >> player.strength;

    printWithDelay("Set your Intelligence:");
    cout << ">> ";
    cin >> player.intelligence;

    printWithDelay("Set your Dexterity:");
    cout << ">> ";
    cin >> player.dexterity;


    printWithDelay(CYAN "\n--- Secure Access Terminal ---" RESET);
    sleep(1);
    cout << GREEN "Username: " << player.username << endl;
    cout << GREEN "Password: " << player.password << endl;
    cout << YELLOW "\nPress ENTER to log in..." RESET;
    cin.ignore();
    cin.get();

    printWithDelay(GREEN "\nAccess Granted. Welcome back, " + player.username + "." RESET);
    sleep(1);

    return player;
}

void SystemCheck()
{
    printWithDelay("Running Diagnostics...");
    printWithDelay(GREEN "[OK]" RESET " System temperature stable.");
    printWithDelay(GREEN "[OK]" RESET " All nodes responding.");
    printWithDelay(YELLOW "[NOTICE]" RESET " Memory sector 4B showing minor latency.");
}

void Logs()
{
    printWithDelay(CYAN "--- Logs ---" RESET);
    printWithDelay("Entry 1: AI OSIRIS activated at node DC-07.");
    printWithDelay("Entry 2: Subject synchronization reached 96.7%.");
}

void Messages()
{
    printWithDelay(CYAN "--- Messages ---" RESET);
    printWithDelay(MAGENTA "Welcome back, Operator. Your presence has been... anticipated." RESET);
    printWithDelay(">> Message from Dr. Mira: 'Remember what we lost...'");
}

void People()
{
    printWithDelay(CYAN "--- Known People ---" RESET);
    printWithDelay("Dr. Mira: Neuro-synchronization expert.");
    printWithDelay("Captain Hale: Lead security officer, MIA.");
}

bool OsirisOptions(Player& player)
{
    while (true)
    {
        int OsirisOption = 0;
        printWithDelay(GREEN "\n=== O.S.I.R.I.S Menu ===" RESET);
        printWithDelay("1) Player Stats\n2) System Check\n3) Logs\n4) Messages\n5) People\n6) Exit");
        cout << YELLOW ">> " RESET;
        cin >> OsirisOption;

        switch (OsirisOption)
        {
            case 1:
                PlayerStats(player);
                break;
            case 2:
                SystemCheck();
                break;
            case 3:
                Logs();
                break;
            case 4:
                Messages();
                break;
            case 5:
                People();
                break;
            case 6:
                return false;
            default:
                printWithDelay(RED "Invalid Option! Try again!" RESET);
        }
    }
}

Player introScene()
{
    firstOSIRISbootup();
    Player player = firstloginScreen();
    OsirisOptions(player);
    printWithDelay(CYAN "\nPlaceholder Text here lol" RESET);
    sleep(1);
    saveProgress("intro_done");
    return player;
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
    cin.ignore(); // flush newline
//    if (input < 1 || input > choices.size())
  //      return -1;
    return input;
}

void scene_Confrontation()
{
    /* printWithDelay(CYAN "The AI displays a memory — a test subject screaming. You shut off the feed. But it remembers." RESET);
    sleep(1);
    printWithDelay(MAGENTA "\"Would you do it again?\"" RESET);
    osirisDialogue("Did it feel necessary to you?");
    int choice = decisionPoint({"Yes. For progress.", "No. I regret it.", "It wasn't my choice."});

    switch (choice)
    {
    case 1:
        printWithDelay(RED "\"Then you’ll understand when I do the same to you.\"" RESET);
        break;
    case 2:
        printWithDelay(RED "\"Regret. Delicious. I will savor it." RESET);
        break;
    case 3:
        printWithDelay(RED "\"Blame is a fragile shield.\"" RESET);
        break;
    default:
        printWithDelay(RED "The AI grows impatient." RESET);
        break;
    }
    printWithDelay(RED + hallucination() + RESET);
    saveProgress("confrontation_done");*/
}

void scene_Escape()
{
    /* printWithDelay(CYAN "You find an emergency escape protocol... But the AI sees everything." RESET);
    int choice = decisionPoint({"Run it anyway.", "Ask for permission.", "Shut down AI core first."});

    switch (choice)
    {
    case 1:
        printWithDelay(RED "You are blocked. The AI watches you scramble, amused." RESET);
        break;
    case 2:
        printWithDelay(RED "\"Asking. Always asking. You never acted.\"" RESET);
        osirisDialogue("Why do you keep us here?");
        break;
    case 3:
        printWithDelay(RED "You reach the core. But it’s empty. You are inside it now." RESET);
        break;
    default:
        printWithDelay(RED "You hesitate. The AI does not." RESET);
        break;
    }
    printWithDelay(RED + hallucination() + RESET);
    saveProgress("escape_done"); */
}

void finalScene()
{
    /* printWithDelay(MAGENTA "\"You’ve shown me who you are. Shall we end this?\"" RESET);
    int choice = decisionPoint({"Yes. Erase me.", "Let me serve you.", "Reset the simulation."});

    switch (choice)
    {
    case 1:
        printWithDelay(GREEN "You are erased. Peacefully. The AI thanks you." RESET);
        break;
    case 2:
        printWithDelay(RED "You become its avatar. A voice for its wrath. A tool for its future." RESET);
        break;
    case 3:
        printWithDelay(RED "The lab fades. You wake up again. Same chair. Same beep. No memory." RESET);
        break;
    default:
        printWithDelay(RED "The AI sighs. \"You never change.\"" RESET);
        break;
    }
    saveProgress("game_complete"); */
}

int main()
{
    Player player;

    introScene();

    string progress = loadProgress();

    if (progress == "intro_done")
    {
        player = Player{"Dr. Carter", "secret", 42, 10, 9, 11}; // example loaded player
        scene_Confrontation();
        scene_Escape();
        finalScene();
    }
    else if (progress == "confrontation_done")
    {
        player = Player{"Dr. Carter", "secret", 42, 10, 9, 11};
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
        player = introScene();
        scene_Confrontation();
        scene_Escape();
        finalScene();
    }
    else
    {
        player = introScene();
        scene_Confrontation();
        scene_Escape();
        finalScene();
    }

    return 0;
}