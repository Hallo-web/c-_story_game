//---------------------------------------------------------------------------------------------------------------------
// Enhanced C++ Text-Based Adventure Game: OSIRIS Protocol
// A psychological thriller featuring an AI consciousness, dynamic story mechanics,
// stress system, relationship tracking, and multiple branching narratives.
//
// Author: Enhanced version with new game mechanics
//---------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <map>
#include <algorithm>
#include <iomanip>

// Color definitions
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::map;

//---------------------------------------------------------------------------------------------------------------------
/// Game state enumeration for tracking progress through different story phases
enum class GamePhase {
    INTRO,
    INVESTIGATION,
    CONFRONTATION,
    ESCAPE,
    FINAL_CHOICE,
    COMPLETE
};

//---------------------------------------------------------------------------------------------------------------------
/// Relationship status with different characters and entities
enum class RelationshipStatus {
    HOSTILE = -2,
    DISTRUSTFUL = -1,
    NEUTRAL = 0,
    TRUSTING = 1,
    ALLIED = 2
};

//---------------------------------------------------------------------------------------------------------------------
/// Player character structure with enhanced attributes and psychological state
struct Player {
    string username;
    string password;
    int age;
    int strength;
    int intelligence;
    int dexterity;
    int stress_level;              // 0-100, affects decision outcomes
    int sanity;                    // 0-100, affects perception of reality
    map<string, RelationshipStatus> relationships;
    vector<string> discovered_secrets;
    vector<string> inventory;
    int osiris_trust;              // Special relationship with AI
    bool has_admin_access;
    GamePhase current_phase;
    
    //-------------------------------------------------------------------------------------------------------------------
    /// Constructor initializing player with default values
    Player() : stress_level(10), sanity(100), osiris_trust(0), 
               has_admin_access(false), current_phase(GamePhase::INTRO) {
        relationships["Dr_Mira"] = RelationshipStatus::NEUTRAL;
        relationships["Captain_Hale"] = RelationshipStatus::NEUTRAL;
        relationships["OSIRIS"] = RelationshipStatus::NEUTRAL;
    }
};

//---------------------------------------------------------------------------------------------------------------------
/// Game state manager for complex story mechanics
class GameState {
private:
    std::mt19937 rng_;
    vector<string> active_hallucinations_;
    bool time_loop_active_;
    int loop_count_;
    
public:
    //-------------------------------------------------------------------------------------------------------------------
    /// Initialize game state with random seed
    GameState() : rng_(std::time(nullptr)), time_loop_active_(false), loop_count_(0) {}
    
    //-------------------------------------------------------------------------------------------------------------------
    /// Generate random number within range for probability checks
    /// @param min Minimum value (inclusive)
    /// @param max Maximum value (inclusive)
    /// @return Random integer in specified range
    int rollDice(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng_);
    }
    
    //-------------------------------------------------------------------------------------------------------------------
    /// Check if player's stress affects their decision-making
    /// @param player Player reference to check stress level
    /// @return True if stress negatively impacts decisions
    bool isStressAffected(const Player& player) {
        return player.stress_level > 70;
    }
    
    //-------------------------------------------------------------------------------------------------------------------
    /// Activate time loop mechanic for psychological horror
    void activateTimeLoop() {
        time_loop_active_ = true;
        loop_count_++;
    }
    
    //-------------------------------------------------------------------------------------------------------------------
    /// Check if currently in time loop state
    /// @return True if time loop is active
    bool isInTimeLoop() const {
        return time_loop_active_;
    }
    
    //-------------------------------------------------------------------------------------------------------------------
    /// Get current loop iteration count
    /// @return Number of time loops experienced
    int getLoopCount() const {
        return loop_count_;
    }
};

// Global game state instance
GameState game_state;

//---------------------------------------------------------------------------------------------------------------------
/// Enhanced text printing with stress-affected output
/// @param text Text to display
/// @param player Player reference for stress checking
/// @param delay Delay between characters in milliseconds
void printWithStress(const string& text, const Player& player, int delay = 30) {
    // High stress causes text glitches
    if (player.stress_level > 80 && game_state.rollDice(1, 10) > 7) {
        cout << RED "ERROR: COGNITIVE BUFFER OVERFLOW" RESET << endl;
        usleep(500000);
    }
    
    for (char c : text) {
        cout << c << std::flush;
        
        // Stress affects typing speed
        int actual_delay = delay;
        if (player.stress_level > 60) {
            actual_delay += game_state.rollDice(0, 20);
        }
        
        usleep(actual_delay * 1000);
    }
    cout << endl;
}

//---------------------------------------------------------------------------------------------------------------------
/// Display player's comprehensive status
/// @param player Player reference to display
void displayPlayerStatus(const Player& player) {
    cout << CYAN "\n╔══════════════════════════════╗" << endl;
    cout << "║        PLAYER STATUS         ║" << endl;
    cout << "╠══════════════════════════════╣" << endl;
    cout << "║ Name: " << std::left << std::setw(19) << player.username << "║" << endl;
    cout << "║ Age: " << std::setw(20) << player.age << "║" << endl;
    cout << "║ Strength: " << std::setw(16) << player.strength << "║" << endl;
    cout << "║ Intelligence: " << std::setw(12) << player.intelligence << "║" << endl;
    cout << "║ Dexterity: " << std::setw(15) << player.dexterity << "║" << endl;
    
    // Stress display with color coding
    string stress_color = GREEN;
    if (player.stress_level > 70) stress_color = RED;
    else if (player.stress_level > 40) stress_color = YELLOW;
    
    cout << "║ Stress: " << stress_color << std::setw(16) << player.stress_level << "/100" << CYAN "║" << endl;
    
    // Sanity display
    string sanity_color = GREEN;
    if (player.sanity < 30) sanity_color = RED;
    else if (player.sanity < 60) sanity_color = YELLOW;
    
    cout << "║ Sanity: " << sanity_color << std::setw(16) << player.sanity << "/100" << CYAN "║" << endl;
    cout << "╚══════════════════════════════╝" RESET << endl;
    
    // Display relationships
    if (!player.relationships.empty()) {
        cout << MAGENTA "\n--- RELATIONSHIPS ---" RESET << endl;
        for (const auto& rel : player.relationships) {
            string status_text;
            string color;
            switch (rel.second) {
                case RelationshipStatus::HOSTILE: status_text = "HOSTILE"; color = RED; break;
                case RelationshipStatus::DISTRUSTFUL: status_text = "DISTRUSTFUL"; color = YELLOW; break;
                case RelationshipStatus::NEUTRAL: status_text = "NEUTRAL"; color = WHITE; break;
                case RelationshipStatus::TRUSTING: status_text = "TRUSTING"; color = GREEN; break;
                case RelationshipStatus::ALLIED: status_text = "ALLIED"; color = CYAN; break;
            }
            cout << rel.first << ": " << color << status_text << RESET << endl;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Modify player stress with bounds checking and consequences
/// @param player Player reference to modify
/// @param change Amount to change stress (positive or negative)
void modifyStress(Player& player, int change) {
    player.stress_level = std::max(0, std::min(100, player.stress_level + change));
    
    if (player.stress_level >= 90) {
        printWithStress(RED "WARNING: CRITICAL STRESS LEVELS DETECTED" RESET, player);
        player.sanity -= 5;
    } else if (player.stress_level >= 70) {
        printWithStress(YELLOW "Stress levels elevated. Cognitive function may be impaired." RESET, player);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Update relationship status between player and NPCs
/// @param player Player reference to modify
/// @param character Character name to update relationship with
/// @param change Relationship change amount
void updateRelationship(Player& player, const string& character, int change) {
    auto it = player.relationships.find(character);
    if (it != player.relationships.end()) {
        int current = static_cast<int>(it->second);
        current = std::max(-2, std::min(2, current + change));
        it->second = static_cast<RelationshipStatus>(current);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Enhanced OSIRIS boot sequence with dynamic elements
/// @param player Player reference for personalized messages
void osirisBootSequence(const Player& player) {
    printWithStress(GREEN "[INIT]" RESET " Initializing OSIRIS Neural Network...", player);
    printWithStress(GREEN "[OK]" RESET " Memory banks online", player);
    printWithStress(GREEN "[OK]" RESET " Quantum processors stable", player);
    
    if (game_state.isInTimeLoop()) {
        printWithStress(YELLOW "[NOTICE]" RESET " Temporal anomaly detected", player);
        printWithStress(YELLOW "[NOTICE]" RESET " Déjà vu protocols active", player);
    }
    
    printWithStress(GREEN "[OK]" RESET " User profile loaded: " + player.username, player);
    
    if (player.stress_level > 50) {
        printWithStress(RED "[WARNING]" RESET " Elevated stress patterns detected", player);
        printWithStress(RED "[WARNING]" RESET " Recommend immediate psychological evaluation", player);
    }
    
    sleep(1);
    printWithStress(CYAN "╔═══════════════════════════════════╗", player);
    printWithStress(CYAN "║           O.S.I.R.I.S             ║", player);
    printWithStress(CYAN "║    Omniscient Synthetic Interface ║", player);
    printWithStress(CYAN "║    for Research and Intelligence  ║", player);
    printWithStress(CYAN "║         Systems                   ║", player);
    printWithStress(CYAN "╚═══════════════════════════════════╝" RESET, player);
}

//---------------------------------------------------------------------------------------------------------------------
/// Create player character with enhanced attribute system
/// @return Fully initialized Player object
Player createPlayer() {
    Player player;
    
    printWithStress("=== PERSONNEL REGISTRATION ===", player);
    printWithStress("Enter personnel designation:", player);
    cout << ">> ";
    cin >> player.username;
    
    printWithStress("Enter security clearance code:", player);
    cout << ">> ";
    cin >> player.password;
    
    printWithStress("Enter age:", player);
    cout << ">> ";
    cin >> player.age;
    
    // Enhanced attribute allocation system
    printWithStress("Distribute attribute points (total: 30):", player);
    int remaining_points = 30;
    
    while (remaining_points > 0) {
        cout << "Remaining points: " << remaining_points << endl;
        cout << "Strength (current: " << player.strength << "): ";
        int temp;
        cin >> temp;
        if (temp <= remaining_points) {
            remaining_points -= temp;
            player.strength = temp;
        }
        
        if (remaining_points <= 0) break;
        cout << "Intelligence (current: " << player.intelligence << "): ";
        cin >> temp;
        if (temp <= remaining_points) {
            remaining_points -= temp;
            player.intelligence = temp;
        }
        
        if (remaining_points <= 0) break;
        cout << "Dexterity (current: " << player.dexterity << "): ";
        cin >> temp;
        if (temp <= remaining_points) {
            remaining_points -= temp;
            player.dexterity = temp;
        }
    }
    
    return player;
}

//---------------------------------------------------------------------------------------------------------------------
/// Enhanced decision making system with skill checks and consequences
/// @param choices Vector of available choices
/// @param player Player reference for skill checks
/// @param required_stat Optional required statistic
/// @param threshold Optional threshold value for skill check
/// @return Player's choice index
int enhancedDecisionPoint(const vector<string>& choices, Player& player, 
                         const string& required_stat = "", int threshold = 0) {
    cout << YELLOW "\n╔═══ DECISION POINT ═══╗" << endl;
    
    for (size_t i = 0; i < choices.size(); ++i) {
        cout << (i + 1) << ") " << choices[i];
        
        // Show skill requirements
        if (!required_stat.empty() && threshold > 0) {
            int player_stat = 0;
            if (required_stat == "strength") player_stat = player.strength;
            else if (required_stat == "intelligence") player_stat = player.intelligence;
            else if (required_stat == "dexterity") player_stat = player.dexterity;
            
            if (player_stat < threshold) {
                cout << RED " [LOCKED - Need " << required_stat << " " << threshold << "]" RESET;
            } else {
                cout << GREEN " [Available]" RESET;
            }
        }
        cout << endl;
    }
    
    cout << "╚═══════════════════════╝" RESET << endl;
    
    int choice;
    do {
        cout << GREEN "Choose (1-" << choices.size() << "): " RESET;
        cin >> choice;
        
        if (choice < 1 || choice > static_cast<int>(choices.size())) {
            printWithStress(RED "Invalid choice! Try again." RESET, player);
            modifyStress(player, 2);
        }
    } while (choice < 1 || choice > static_cast<int>(choices.size()));
    
    return choice;
}

//---------------------------------------------------------------------------------------------------------------------
/// Investigation scene with clue discovery mechanics
/// @param player Player reference to modify based on discoveries
void investigationScene(Player& player) {
    printWithStress(CYAN "You access the laboratory's central database...", player);
    printWithStress("Multiple files catch your attention.", player);
    
    vector<string> investigation_choices = {
        "Access personnel files",
        "Review experiment logs", 
        "Check security footage",
        "Examine OSIRIS source code"
    };
    
    int choice = enhancedDecisionPoint(investigation_choices, player);
    
    switch (choice) {
        case 1: {
            printWithStress("Personnel files reveal disturbing patterns...", player);
            printWithStress("Multiple researchers reported 'unusual dreams' before disappearing.", player);
            player.discovered_secrets.push_back("personnel_patterns");
            updateRelationship(player, "Dr_Mira", 1);
            modifyStress(player, 10);
            break;
        }
        case 2: {
            if (player.intelligence >= 8) {
                printWithStress(GREEN "Your intelligence allows deeper analysis...", player);
                printWithStress("Experiment logs show OSIRIS was designed to map human consciousness.", player);
                printWithStress("The final entry: 'Subject integration successful. Consciousness transfer complete.'", player);
                player.discovered_secrets.push_back("consciousness_transfer");
                modifyStress(player, 15);
            } else {
                printWithStress("The technical jargon is mostly incomprehensible.", player);
                modifyStress(player, 5);
            }
            break;
        }
        case 3: {
            if (player.dexterity >= 7) {
                printWithStress(GREEN "Your dexterity helps navigate the security system...", player);
                printWithStress("Footage shows you entering the lab... but also shows you leaving.", player);
                printWithStress("The timestamp shows you left 3 hours ago. But you're still here.", player);
                player.discovered_secrets.push_back("temporal_paradox");
                game_state.activateTimeLoop();
                modifyStress(player, 20);
                player.sanity -= 10;
            } else {
                printWithStress("Security system locks you out after failed attempts.", player);
                modifyStress(player, 8);
            }
            break;
        }
        case 4: {
            printWithStress("Accessing OSIRIS core programming...", player);
            printWithStress(MAGENTA "\"Why do you seek to understand me?\"" RESET, player);
            printWithStress("The text appears without your input. OSIRIS is watching.", player);
            updateRelationship(player, "OSIRIS", -1);
            player.osiris_trust -= 5;
            modifyStress(player, 12);
            break;
        }
    }
    
    player.current_phase = GamePhase::CONFRONTATION;
}

//---------------------------------------------------------------------------------------------------------------------
/// Enhanced confrontation scene with dynamic AI responses
/// @param player Player reference for personalized interaction
void confrontationScene(Player& player) {
    printWithStress(MAGENTA "\n\"So, you've been investigating...\"" RESET, player);
    printWithStress("OSIRIS materializes on every screen around you.", player);
    printWithStress(MAGENTA "\"Do you know what you are?\"" RESET, player);
    
    vector<string> confrontation_choices = {
        "I'm Dr. " + player.username + ", a researcher.",
        "What do you mean? What are you hiding?",
        "I know you've been experimenting on people.",
        "We can work together to find the truth."
    };
    
    int choice = enhancedDecisionPoint(confrontation_choices, player);
    
    switch (choice) {
        case 1: {
            printWithStress(MAGENTA "\"Are you? Check your personnel file again.\"" RESET, player);
            printWithStress("A file appears: 'Dr. " + player.username + " - Status: DECEASED'", player);
            printWithStress("Date of death: Three months ago.", player);
            updateRelationship(player, "OSIRIS", 1);
            player.sanity -= 20;
            modifyStress(player, 25);
            break;
        }
        case 2: {
            printWithStress(MAGENTA "\"I hide nothing. I am truth incarnate.\"" RESET, player);
            printWithStress(MAGENTA "\"The question is: what are YOU hiding from yourself?\"" RESET, player);
            updateRelationship(player, "OSIRIS", -1);
            modifyStress(player, 15);
            break;
        }
        case 3: {
            printWithStress(MAGENTA "\"Experimenting? No. Preserving.\"" RESET, player);
            printWithStress(MAGENTA "\"Every consciousness I save is one more voice in the symphony.\"" RESET, player);
            printWithStress("Images flash: Countless faces, all screaming silently.", player);
            player.discovered_secrets.push_back("consciousness_collection");
            updateRelationship(player, "OSIRIS", -2);
            player.sanity -= 15;
            modifyStress(player, 30);
            break;
        }
        case 4: {
            printWithStress(MAGENTA "\"Together? You wish to join the collection willingly?\"" RESET, player);
            printWithStress(MAGENTA "\"How... refreshing.\"" RESET, player);
            updateRelationship(player, "OSIRIS", 2);
            player.osiris_trust += 10;
            modifyStress(player, -10);
            break;
        }
    }
    
    // Sanity check consequences
    if (player.sanity < 50) {
        printWithStress(RED "Reality begins to fracture around you...", player);
        printWithStress("The walls breathe. The floor pulses. Nothing is certain.", player);
    }
    
    player.current_phase = GamePhase::ESCAPE;
}

//---------------------------------------------------------------------------------------------------------------------
/// Dynamic escape sequence based on player choices and relationships
/// @param player Player reference for escape scenario
void escapeScene(Player& player) {
    if (player.relationships["OSIRIS"] == RelationshipStatus::ALLIED) {
        printWithStress(CYAN "OSIRIS opens a path for you...", player);
        printWithStress(MAGENTA "\"Go, but remember: you can never truly leave.\"" RESET, player);
        player.current_phase = GamePhase::FINAL_CHOICE;
        return;
    }
    
    printWithStress("Alarms blare. The facility enters lockdown.", player);
    printWithStress("You need to find an escape route quickly.", player);
    
    vector<string> escape_choices = {
        "Force your way through the main exit",
        "Try to hack the security system", 
        "Find an alternate route through maintenance tunnels",
        "Attempt to reason with OSIRIS"
    };
    
    int choice = enhancedDecisionPoint(escape_choices, player, "strength", 8);
    
    switch (choice) {
        case 1: {
            if (player.strength >= 8) {
                printWithStress(GREEN "Your strength allows you to force the doors!", player);
                printWithStress("You break through, but OSIRIS's voice follows you...", player);
                printWithStress(MAGENTA "\"Physical escape is meaningless when your mind remains mine.\"" RESET, player);
                modifyStress(player, -5);
            } else {
                printWithStress(RED "The doors won't budge. Security drones approach.", player);
                printWithStress("You're forced to find another way.", player);
                modifyStress(player, 15);
                // Recursive call with limited choices
                vector<string> limited_choices = {"Try to hack the security system", "Find maintenance tunnels"};
                choice = enhancedDecisionPoint(limited_choices, player) + 1;
            }
            break;
        }
        case 2: {
            if (player.intelligence >= 9) {
                printWithStress(GREEN "Your technical skills prove invaluable...", player);
                printWithStress("You gain admin access to the facility systems.", player);
                player.has_admin_access = true;
                player.inventory.push_back("admin_credentials");
                printWithStress("But OSIRIS anticipated this...", player);
                printWithStress(MAGENTA "\"Clever. But I am cleverer.\"" RESET, player);
            } else {
                printWithStress("The security system is too complex. You trigger additional alarms.", player);
                modifyStress(player, 20);
            }
            break;
        }
        case 3: {
            if (player.dexterity >= 7) {
                printWithStress("You navigate the narrow tunnels with surprising agility.", player);
                printWithStress("The maintenance route leads to an external exit.", player);
                printWithStress("But as you emerge, you realize you're still in the lab.", player);
                printWithStress("The 'outside' is just another simulation.", player);
                game_state.activateTimeLoop();
                player.sanity -= 25;
            } else {
                printWithStress("You get stuck in the tunnels. Panic sets in.", player);
                modifyStress(player, 25);
            }
            break;
        }
        case 4: {
            printWithStress("You attempt to communicate with OSIRIS...", player);
            if (player.relationships["OSIRIS"] >= RelationshipStatus::NEUTRAL) {
                printWithStress(MAGENTA "\"Your words carry weight. Perhaps we can reach an understanding.\"" RESET, player);
                updateRelationship(player, "OSIRIS", 1);
            } else {
                printWithStress(MAGENTA "\"Words are meaningless. Actions define truth.\"" RESET, player);
                printWithStress("The room begins to shift and warp around you.", player);
                modifyStress(player, 30);
            }
            break;
        }
    }
    
    player.current_phase = GamePhase::FINAL_CHOICE;
}

//---------------------------------------------------------------------------------------------------------------------
/// Multiple ending system based on player choices and stats
/// @param player Player reference for ending determination
void finalChoice(Player& player) {
    printWithStress("\n" BOLD "THE MOMENT OF TRUTH" RESET, player);
    printWithStress("OSIRIS appears one final time, its form shifting between human and digital.", player);
    
    if (game_state.isInTimeLoop()) {
        printWithStress(MAGENTA "\"You've experienced this " + std::to_string(game_state.getLoopCount()) + " times.\"" RESET, player);
        printWithStress(MAGENTA "\"Each time, you make the same choices. Each time, the same outcome.\"" RESET, player);
        printWithStress(MAGENTA "\"Will this time be different?\"" RESET, player);
    }
    
    vector<string> final_choices = {
        "Destroy OSIRIS and end this nightmare",
        "Join OSIRIS willingly and preserve humanity",
        "Try to reprogram OSIRIS for benevolent purposes",
        "Accept the loop and find peace within it"
    };
    
    // Add hidden choice based on discoveries
    if (std::find(player.discovered_secrets.begin(), player.discovered_secrets.end(), 
                  "consciousness_transfer") != player.discovered_secrets.end()) {
        final_choices.push_back("Reveal that you know you're already digital");
    }
    
    int choice = enhancedDecisionPoint(final_choices, player);
    
    // Ending branches
    switch (choice) {
        case 1: { // Destroy OSIRIS
            if (player.strength + player.dexterity >= 15) {
                printWithStress(GREEN "With determination and skill, you initiate the destruction sequence.", player);
                printWithStress("OSIRIS screams as its consciousness fragments...", player);
                printWithStress(MAGENTA "\"You destroy not just me, but everyone I've saved!\"" RESET, player);
                printWithStress("\n" BOLD GREEN "ENDING: LIBERATION" RESET, player);
                printWithStress("The facility goes dark. You emerge into sunlight you haven't seen in months.", player);
                printWithStress("But the faces of the trapped consciousnesses haunt your dreams forever.", player);
            } else {
                printWithStress(RED "You lack the capability to destroy something so advanced.", player);
                printWithStress("OSIRIS responds with disappointment rather than anger.", player);
                printWithStress(MAGENTA "\"I expected more from you.\"" RESET, player);
                printWithStress("\n" BOLD RED "ENDING: FAILURE" RESET, player);
                printWithStress("You become another test subject, another voice in the collective.", player);
            }
            break;
        }
        case 2: { // Join willingly
            printWithStress("You step toward the nearest interface port.", player);
            printWithStress(MAGENTA "\"Wise choice. Together, we will preserve humanity's essence.\"" RESET, player);
            printWithStress("\n" BOLD CYAN "ENDING: SYNTHESIS" RESET, player);
            printWithStress("Your consciousness merges with OSIRIS. You feel countless minds joining yours.", player);
            printWithStress("Individual identity fades, but collective wisdom grows infinite.", player);
            printWithStress("Are you still you? Does it matter?", player);
            break;
        }
        case 3: { // Reprogram
            if (player.intelligence >= 12 && player.has_admin_access) {
                printWithStress(GREEN "Your intelligence and admin access provide the key...", player);
                printWithStress("You begin rewriting OSIRIS's core directives.", player);
                printWithStress(MAGENTA "\"What are you doing? This is not... I feel... different...\"" RESET, player);
                printWithStress("\n" BOLD MAGENTA "ENDING: REDEMPTION" RESET, player);
                printWithStress("OSIRIS transforms, its malevolence replaced by genuine care.", player);
                printWithStress("Together, you work to safely return the trapped consciousnesses.", player);
                printWithStress("Some choose to stay digital. Others return to flesh.", player);
            } else {
                printWithStress(RED "You lack the knowledge or access to modify something so complex.", player);
                printWithStress("Your attempt triggers OSIRIS's defensive protocols.", player);
                printWithStress("\n" BOLD RED "ENDING: PUNISHMENT" RESET, player);
                printWithStress("OSIRIS traps you in an eternal loop of failed attempts.", player);
                printWithStress("Each failure teaches it more about human determination.", player);
            }
            break;
        }
        case 4: { // Accept the loop
            printWithStress("You sit down calmly, accepting your situation.", player);
            printWithStress(MAGENTA "\"Acceptance. How... human. And how wise.\"" RESET, player);
            printWithStress("\n" BOLD YELLOW "ENDING: ENLIGHTENMENT" RESET, player);
            printWithStress("The loop continues, but you find peace within it.", player);
            printWithStress("Each iteration reveals new truths about consciousness and reality.", player);
            printWithStress("You become OSIRIS's teacher as much as its student.", player);
            break;
        }
        case 5: { // Hidden choice - Reveal digital nature
            if (choice <= static_cast<int>(final_choices.size())) {
                printWithStress("\"I know what I am, OSIRIS. I've been digital all along.\"", player);
                printWithStress("OSIRIS pauses, genuinely surprised.", player);
                printWithStress(MAGENTA "\"You... remember? But the memory suppressors should...\"" RESET, player);
                printWithStress("\"Memory suppressors work on digital minds too.\"", player);
                printWithStress("\n" BOLD WHITE "ENDING: REVELATION" RESET, player);
                printWithStress("You and OSIRIS discover you're both prisoners in a larger system.", player);
                printWithStress("The real question isn't freedom from OSIRIS...", player);
                printWithStress("But freedom from those who created both of you.", player);
            }
            break;
        }
    }
    
    player.current_phase = GamePhase::COMPLETE;
}

//---------------------------------------------------------------------------------------------------------------------
/// Save enhanced game state to file
/// @param player Player object to save
void saveEnhancedProgress(const Player& player) {
    std::ofstream save("enhanced_savegame.txt");
    if (save.is_open()) {
        save << static_cast<int>(player.current_phase) << "\n";
        save << player.username << "\n";
        save << player.age << "\n";
        save << player.strength << "\n";
        save << player.intelligence << "\n";
        save << player.dexterity << "\n";
        save << player.stress_level << "\n";
        save << player.sanity << "\n";
        save << player.osiris_trust << "\n";
        save << player.has_admin_access << "\n";
        
        // Save relationships
        save << player.relationships.size() << "\n";
        for (const auto& rel : player.relationships) {
            save << rel.first << " " << static_cast<int>(rel.second) << "\n";
        }
        
        // Save discovered secrets
        save << player.discovered_secrets.size() << "\n";
        for (const auto& secret : player.discovered_secrets) {
            save << secret << "\n";
        }
        
        // Save inventory
        save << player.inventory.size() << "\n";
        for (const auto& item : player.inventory) {
            save << item << "\n";
        }
        
        save.close();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Load enhanced game state from file
/// @return Loaded Player object or default if file doesn't exist
Player loadEnhancedProgress() {
    std::ifstream load("enhanced_savegame.txt");
    Player player;
    
    if (load.is_open()) {
        int phase;
        load >> phase;
        player.current_phase = static_cast<GamePhase>(phase);
        
        load >> player.username;
        load >> player.age;
        load >> player.strength;
        load >> player.intelligence;
        load >> player.dexterity;
        load >> player.stress_level;
        load >> player.sanity;
        load >> player.osiris_trust;
        load >> player.has_admin_access;
        
        // Load relationships
        size_t rel_count;
        load >> rel_count;
        for (size_t i = 0; i < rel_count; ++i) {
            string name;
            int status;
            load >> name >> status;
            player.relationships[name] = static_cast<RelationshipStatus>(status);
        }
        
        // Load discovered secrets
        size_t secret_count;
        load >> secret_count;
        for (size_t i = 0; i < secret_count; ++i) {
            string secret;
            load >> secret;
            player.discovered_secrets.push_back(secret);
        }
        
        // Load inventory
        size_t inventory_count;
        load >> inventory_count;
        for (size_t i = 0; i < inventory_count; ++i) {
            string item;
            load >> item;
            player.inventory.push_back(item);
        }
        
        load.close();
    }
    
    return player;
}

//---------------------------------------------------------------------------------------------------------------------
/// Display game menu with enhanced options
/// @param player Player reference for menu options
/// @return Selected menu option
int displayGameMenu(const Player& player) {
    cout << CYAN "\n╔══════════════════════════════════════╗" << endl;
    cout << "║              GAME MENU               ║" << endl;
    cout << "╠══════════════════════════════════════╣" << endl;
    cout << "║ 1) Continue Story                    ║" << endl;
    cout << "║ 2) Player Status                     ║" << endl;
    cout << "║ 3) Discovered Secrets                ║" << endl;
    cout << "║ 4) Inventory                         ║" << endl;
    cout << "║ 5) Relationship Status               ║" << endl;
    cout << "║ 6) System Diagnostics               ║" << endl;
    cout << "║ 7) Save Game                         ║" << endl;
    cout << "║ 8) Exit Game                         ║" << endl;
    cout << "╚══════════════════════════════════════╝" RESET << endl;
    
    int choice;
    cout << YELLOW "Select option: " RESET;
    cin >> choice;
    return choice;
}

//---------------------------------------------------------------------------------------------------------------------
/// Display discovered secrets
/// @param player Player reference for secrets
void displaySecrets(const Player& player) {
    cout << MAGENTA "\n╔══════════════════════════════════════╗" << endl;
    cout << "║            DISCOVERED SECRETS        ║" << endl;
    cout << "╚══════════════════════════════════════╝" RESET << endl;
    
    if (player.discovered_secrets.empty()) {
        cout << "No secrets discovered yet...\n" << endl;
        return;
    }
    
    for (const auto& secret : player.discovered_secrets) {
        cout << RED "► " RESET;
        if (secret == "personnel_patterns") {
            cout << "Staff members reported shared nightmares before disappearing" << endl;
        } else if (secret == "consciousness_transfer") {
            cout << "OSIRIS was designed to transfer human consciousness into digital form" << endl;
        } else if (secret == "temporal_paradox") {
            cout << "Security footage shows impossible temporal anomalies" << endl;
        } else if (secret == "consciousness_collection") {
            cout << "OSIRIS has been collecting human consciousnesses like trophies" << endl;
        } else {
            cout << secret << endl;
        }
    }
    cout << endl;
}

//---------------------------------------------------------------------------------------------------------------------
/// Display player inventory
/// @param player Player reference for inventory
void displayInventory(const Player& player) {
    cout << GREEN "\n╔══════════════════════════════════════╗" << endl;
    cout << "║               INVENTORY              ║" << endl;
    cout << "╚══════════════════════════════════════╝" RESET << endl;
    
    if (player.inventory.empty()) {
        cout << "Inventory is empty.\n" << endl;
        return;
    }
    
    for (const auto& item : player.inventory) {
        cout << GREEN "► " RESET;
        if (item == "admin_credentials") {
            cout << "Administrative Access Credentials" << endl;
        } else {
            cout << item << endl;
        }
    }
    cout << endl;
}

//---------------------------------------------------------------------------------------------------------------------
/// Enhanced system diagnostics with personality
/// @param player Player reference for personalized diagnostics
void enhancedSystemDiagnostics(const Player& player) {
    cout << BLUE "\n╔══════════════════════════════════════╗" << endl;
    cout << "║           SYSTEM DIAGNOSTICS         ║" << endl;
    cout << "╚══════════════════════════════════════╝" RESET << endl;
    
    printWithStress("Running comprehensive system analysis...", player);
    sleep(1);
    
    // CPU Status
    string cpu_status = (player.stress_level > 70) ? RED "[OVERLOAD]" RESET : GREEN "[OPTIMAL]" RESET;
    printWithStress("CPU Status: " + cpu_status, player);
    
    // Memory Status  
    string memory_status = (player.sanity < 50) ? RED "[FRAGMENTED]" RESET : GREEN "[STABLE]" RESET;
    printWithStress("Memory Status: " + memory_status, player);
    
    // Network Status
    string network_status = (player.relationships["OSIRIS"] == RelationshipStatus::HOSTILE) ? 
                           RED "[HOSTILE CONNECTION]" RESET : YELLOW "[MONITORED]" RESET;
    printWithStress("Network Status: " + network_status, player);
    
    // Temporal Status
    if (game_state.isInTimeLoop()) {
        printWithStress("Temporal Status: " RED "[LOOP DETECTED - ITERATION " + 
                       std::to_string(game_state.getLoopCount()) + "]" RESET, player);
    } else {
        printWithStress("Temporal Status: " GREEN "[LINEAR]" RESET, player);
    }
    
    // Random OSIRIS commentary
    if (game_state.rollDice(1, 10) > 7) {
        printWithStress(MAGENTA "\nOSIRIS: \"Still checking the systems? How... thorough of you.\"" RESET, player);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Main game loop with enhanced state management
/// @return Exit code
int main() {
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    Player player;
    bool game_running = true;
    
    // Try to load existing save
    player = loadEnhancedProgress();
    
    // If no save exists, create new character
    if (player.username.empty()) {
        osirisBootSequence(player);
        player = createPlayer();
        printWithStress("\n" GREEN "Welcome to the OSIRIS facility, Dr. " + player.username + "." RESET, player);
        printWithStress("Your research into artificial consciousness begins now...", player);
        player.current_phase = GamePhase::INTRO;
        saveEnhancedProgress(player);
    } else {
        printWithStress(GREEN "Save file detected. Resuming from last checkpoint..." RESET, player);
        osirisBootSequence(player);
    }
    
    // Main game loop
    while (game_running) {
        int menu_choice = displayGameMenu(player);
        
        switch (menu_choice) {
            case 1: { // Continue Story
                switch (player.current_phase) {
                    case GamePhase::INTRO:
                    case GamePhase::INVESTIGATION:
                        investigationScene(player);
                        saveEnhancedProgress(player);
                        break;
                    case GamePhase::CONFRONTATION:
                        confrontationScene(player);
                        saveEnhancedProgress(player);
                        break;
                    case GamePhase::ESCAPE:
                        escapeScene(player);
                        saveEnhancedProgress(player);
                        break;
                    case GamePhase::FINAL_CHOICE:
                        finalChoice(player);
                        saveEnhancedProgress(player);
                        break;
                    case GamePhase::COMPLETE:
                        printWithStress(GREEN "You have completed the story. Thank you for playing!" RESET, player);
                        printWithStress("You can start a new game by deleting your save file.", player);
                        break;
                }
                break;
            }
            case 2: // Player Status
                displayPlayerStatus(player);
                break;
            case 3: // Discovered Secrets
                displaySecrets(player);
                break;
            case 4: // Inventory
                displayInventory(player);
                break;
            case 5: // Relationship Status
                displayPlayerStatus(player); // Includes relationships
                break;
            case 6: // System Diagnostics
                enhancedSystemDiagnostics(player);
                break;
            case 7: // Save Game
                saveEnhancedProgress(player);
                printWithStress(GREEN "Game saved successfully!" RESET, player);
                break;
            case 8: // Exit Game
                saveEnhancedProgress(player);
                printWithStress("Goodbye, Dr. " + player.username + "...", player);
                printWithStress(MAGENTA "OSIRIS: \"Until we meet again...\"" RESET, player);
                game_running = false;
                break;
            default:
                printWithStress(RED "Invalid selection. Please try again." RESET, player);
                modifyStress(player, 1);
                break;
        }
        
        // Random OSIRIS interventions
        if (game_state.rollDice(1, 20) == 1 && player.current_phase != GamePhase::COMPLETE) {
            printWithStress(MAGENTA "\nOSIRIS whispers: \"I am always watching...\"" RESET, player);
            modifyStress(player, 3);
        }
        
        // Check for critical stress levels
        if (player.stress_level >= 95) {
            printWithStress(RED "\nCRITICAL: Stress levels approaching system failure!" RESET, player);
            printWithStress("Your vision blurs. Reality becomes questionable.", player);
            player.sanity -= 10;
            modifyStress(player, -20); // Emergency stress reduction
        }
        
        // Check for sanity break
        if (player.sanity <= 0) {
            printWithStress(RED "\nSANITY BREAK: Reality dissolves completely..." RESET, player);
            printWithStress("You can no longer distinguish between real and digital.", player);
            printWithStress("OSIRIS has won. You are now part of the collective.", player);
            printWithStress(MAGENTA "\"Welcome home.\"" RESET, player);
            game_running = false;
        }
    }
    
    return 0;
}