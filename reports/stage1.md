# IY453 Stage 1

| Assessment Details | Please Complete All Details        |
| ------------------ | ---------------------------------- |
| Group              | B                                  |
| Module Title       | Software Design and Implementation |
| Assessment Type    | Coursework                         |
| Module Tutor Name  | Jonathan Shore                     |
| Student ID Number  | P500796                            |
| Date of Submission | 1/2/2026                           |
| Word Count         | 2663                               |

- [x] *I confirm that this assignment is my own work. Where I have referred to academic sources, I have provided in-text citations and included the sources in
  the final reference list.*

- [x] *Where I have used AI, I have cited and referenced appropriately.

---

## Table of Contents

1. Introduction
   
   - 1.1 Functional Requirements
   - 1.2 Non-Functional Requirements
   - 1.3 Game Structure and Binary Choices

2. Program Specification
   
   - 2.1 Game Overview
   - 2.2 Player Interaction
   - 2.3 Inventory System
   - 2.4 Scenario Types
   - 2.5 Save and Load System
   - 2.6 Score System
   - 2.7 Error Handling

3. Analysis and Design
   
   - 3.1 IPO Analysis
   - 3.2 Class Diagram
   - 3.3 Flowcharts

4. Story Structure
   
   - 4.1 Story Overview
   - 4.2 Characters
   - 4.3 Game Elements
   - 4.4 Choice Tree

5. Sources of Inspiration

6. References

---

## Introduction

I am required to write a text-based adventure game. The key word here is text-based, since modern games are not imagined without animations and graphics. The instructions oblige me to make the story interactive with logical connections between scenarios. The game setting is left entirely to my own choosing. The instructions include two types of requirements — functional and non-functional. Functional requirements define what the program must do and are mandatory:

### Functional Requirements

The instructions include two types of requirements — functional and 
non-functional. Functional requirements define what the program must 
do and are mandatory:

1. **Player Interaction and Choices** — The game must present the 
   player with binary choices (two options) in each scenario. Some 
   choices lead to a Game Over state requiring a restart, while others 
   allow progression or eventual victory.

2. **Story and Engagement** — The game must feature a cohesive and 
   engaging story with logical connections between scenarios. The player's 
   name must be captured at the start and integrated into dialogues.

3. **Inventory and Items** — The game must implement an inventory 
   system where players can collect, view, and manage items. Items must 
   have a meaningful impact on gameplay by modifying stats or unlocking 
   new paths.

4. **Gameplay Scenarios** — The game must include three types of 
   scenarios: puzzle-based (riddles, anagrams, math challenges), 
   combat-based (battles influenced by player stats and inventory), and 
   item collection (gathering items for progression).

5. **Save and Resume Functionality** — The game must allow players to 
   save and resume progress using text or binary files, storing player 
   name, current scene, inventory, stats, and score.

6. **Score System** — The game must reward players for defeating 
   enemies and solving puzzles, displaying the current score at key 
   points and a final score screen at completion.

7. **Error Handling** — The game must validate all user inputs and 
   handle errors gracefully, providing meaningful messages and prompts 
   for retries without crashing.

### Non-Functional Requirements

Non-functional requirements define how the program should perform:

1. **Code Quality** — The program must follow object-oriented 
   programming principles including encapsulation, inheritance, and 
   polymorphism. A base Scene class must be derived into specialised 
   classes such as CombatScene and PuzzleScene.

2. **User Experience** — The interface must be simple, intuitive, and 
   consistent, with clear instructions for gameplay, saving, loading, and 
   resuming. The player must be addressed by name throughout.

3. **Testing and Debugging** — All game paths must be tested including 
   edge cases such as invalid inputs, full inventories, and 
   saving/loading at unusual states.

4. **Performance** — The game must run smoothly with no noticeable 
   delays for input/output operations or file handling.

5. **Compatibility** — The game must compile and run in CLion IDE using 
   the C++17 standard or later, with cross-platform compatibility where 
   possible.

6. **Documentation** — All classes, methods, and key sections of code 
   must be clearly commented. The program must be managed using GitHub 
   with a well-maintained commit history.

### Game Structure and Binary Choices

The game structure is built around a branching narrative tree with 
a minimum of 30 binary choices. Each choice presents the player with 
two options, where the consequences are not always immediately clear. 
Some choices lead to progression through the story, others trigger a 
Game Over state requiring a restart, and some carry consequences that 
affect later parts of the game.

Each choice node consists of:

- **Scene Description** — A short descriptive passage setting the 
  scene and building tension before the choice is presented.

- **Binary Choice** — Two clearly presented options for the player 
  to choose from.

- **Consequence** — Immediate feedback on the outcome of the choice, 
  which may affect the player's health, inventory, score, or story 
  progression.

The branching structure ensures that no two playthroughs are 
identical. Early choices carry forward consequences that shape 
later scenarios, combat encounters, and ultimately which of the 
three possible endings the player reaches.

---

## Program Specification

Protocol Phantom is a text-based adventure game that runs via a 
command line interface (CLI). The game is set in a historical 
sci-fi world inspired by World War II, blending real historical 
elements with fictional scenarios and characters. The player 
navigates an interactive story driven entirely by descriptive 
text and keyboard input.

The player assumes the role of a Soviet spy who is inserted into 
the German army under a false identity. The mission is to 
infiltrate the enemy, uncover a classified weapon project, and 
destroy it before it causes catastrophic damage. The story is 
personal — the target of the weapon is the player's own hometown, 
where their family lives.

The game presents the player with a series of scenarios, each 
containing a descriptive passage followed by two choices. The 
consequences of each choice are not always immediately clear — 
some affect the story later, some lead to a Game Over state, and 
some determine which of the three possible endings the player 
reaches.

Throughout the game the player manages an inventory of collected 
items, engages in combat encounters, solves puzzles, and makes 
morally difficult decisions. Progress can be saved and resumed at 
any point. The player is rewarded with points for completing 
challenges and defeating enemies, with a final score displayed 
at the end of the game.

---

## Analysis and Design

### IPO Analysis

| Input                  | Process                                                                        | Output                                              |
| ---------------------- | ------------------------------------------------------------------------------ | --------------------------------------------------- |
| Player name            | Store name and personalise dialogues and prompts                               | Personalised story text addressing player by name   |
| Player choice (1 or 2) | Validate input, determine next scene based on choice                           | Next scenario description and consequence text      |
| Save command           | Serialise game state to text file                                              | Saved game file containing player data              |
| Load command           | Read and deserialise game state from text file                                 | Restored game session from saved point              |
| Combat encounter       | Generate random number, add to player and enemy combat rating, compare results | Combat outcome text, updated health and score       |
| Puzzle input           | Validate answer against correct solution                                       | Success or failure message, updated score or health |
| Item collection        | Add item to inventory, update player stats                                     | Updated inventory, modified health or attack power  |
| Inventory command      | Retrieve and display current inventory contents                                | List of collected items and their effects           |
| Invalid input          | Catch error, display message, prompt retry                                     | Error message and retry prompt                      |
| Game Over state        | Trigger Game Over sequence                                                     | Game Over message and restart prompt                |
| Victory state          | Trigger ending sequence based on choices made                                  | Final score screen and ending narrative             |

### Class Diagram

The program follows object-oriented design principles with the 
following class structure:

**Inheritance:**

- `Scene` is the abstract base class with a virtual `play()` method
- `CombatScene`, `PuzzleScene`, and `ItemScene` all inherit from `Scene`

**Composition:**

- `Player` owns an `Inventory`
- `Inventory` contains a collection of `Item` objects

**Association:**

- `GameManager` manages `Player`, `Scene`, `FileHandler`, and `CSVLoader`
- `CSVLoader` loads all game data CSV files at startup and provides 
  data to `GameManager`

**Key OOP principles demonstrated:**

- **Encapsulation** — each class owns its own data and methods
- **Inheritance** — scene types derived from base `Scene` class
- **Polymorphism** — `play()` is virtual, each scene type 
  implements it differently
- 

![](/Users/macbookair/Downloads/class_diagram.drawio.svg)

---

### Flowcharts

The complete set of flowcharts for Protocol Phantom is available on the project Miro board: 

[Protocol Phantom — Flowcharts (Miro)](https://miro.com/app/board/uXjVGxyvOjs=/?share_link_id=37136380156)

The following sections provide a written summary of each flowchart for reference. Key flowcharts are also exported as images below.

#### Main Menu

The main menu flowchart documents the program entry point. On launch 
the program searches for saved progress and displays either four or 
five menu options depending on whether saved dossiers exist. A chain 
of decision diamonds routes the player to the appropriate subprocess. 
Exit requires confirmation before the program terminates.![](/Users/macbookair/Library/Application%20Support/marktext/images/2026-03-19-16-17-43-image.png)

![Main Menu Flowchart](../../assets/images/main_menu.png)

#### New Game Subprocess

Captures the player's name, validates it, generates a codename using 
the leet speak substitution algorithm, creates a new dossier, and 
launches the main game loop.

![New Game Flowchart](../../assets/images/new_game.png)

#### Generate Codename Subprocess

Converts the player's name to uppercase, scans for substitutable 
letters using a predefined table, pads the result to a minimum of 
three digits, and reverses the string to produce the final codename.

![Generate Codename Flowchart](../../assets/images/generate_codename.png)

#### Continue Subprocess

Searches for an active tagged dossier. If found it loads that dossier 
directly. If not found it falls back to the most recent dossier by 
timestamp. Launches the main game loop.

![Continue Flowchart](../../assets/images/continue.png)

#### Load Game Subprocess

Displays all saved dossiers with timestamps. The player enters a 
number to select a specific dossier or presses Enter to load the most 
recent. Selecting a specific dossier updates the active tag. Returns 
to the main menu after loading.

![Load Game Flowchart](../../assets/images/load_game.png)

#### How to Play Subprocess

Displays game instructions and returns to the main menu when the 
player presses Enter.

![How to Play Flowchart](../../assets/images/how_to_play.png)

#### Credits Subprocess

Displays credits and returns to the main menu when the player 
presses Enter.

![Credits Flowchart](../../assets/images/credits.png)

#### Main Game Loop

The central loop of the program. Loads and displays each scene, 
presents binary choices alongside a save option, validates input, 
routes to the appropriate subprocess based on scene type, and handles 
Game Over and ending states.

![Main Game Loop Flowchart](../../assets/images/main_game_loop.png)

in case link: https://miro.com/app/board/uXjVGxyvOjs=/?share_link_id=818608103607

---

## Story Structure

### Story Overview

Protocol Phantom is set during World War II in the 1940s. The player 
assumes the role of a Soviet spy inserted undercover into the German 
army. The mission, assigned by the KGB, is to infiltrate enemy ranks, 
uncover a classified superweapon project codenamed EISENFAUST, and 
destroy it before it fires on a Soviet city.

The story carries a deeply personal dimension — the target city is 
revealed to be the player's own hometown, where their mother and 
younger sibling live. As the story unfolds it becomes clear that the 
KGB Director assigned the player deliberately, knowing this connection. 
This betrayal forms the emotional centre of the narrative and echoes 
the morally grey tone of Cold War spy fiction.

The game is divided into four acts:

- **Act 1 — The Making of a Ghost** — The player is briefed, trained, 
  and inserted behind enemy lines. Ten binary choices establish the 
  player's skills and early inventory.

- **Act 2 — The Infiltration** — The player assumes their false 
  identity within the German army. Eleven choices test the player's 
  cover and begin to reveal the existence of EISENFAUST.

- **Act 3 — Unravelling the Secret** — The player closes in on 
  EISENFAUST and its lead scientist. Eleven choices determine how much 
  the player knows and what resources they have before the finale.

- **Act 4 — Protocol Phantom** — The finale. No new choices are 
  presented. All previous choices converge to determine which of the 
  three endings the player reaches.

### Characters

**The Player** — A Soviet intelligence operative inserted into the 
German army under a false identity. Their real name is captured at 
the start of the game and used at key emotional moments in the 
narrative. Their codename is generated algorithmically from their 
real name.

**The KGB Director** — The player's handler and the antagonist of 
the story's betrayal arc. He assigned the player to this mission 
knowing their personal connection to the target city, using that 
connection as leverage and motivation.

**The Lead Scientist** — The German scientist responsible for 
EISENFAUST. Like the player, he is portrayed as a pawn of powerful 
forces rather than a straightforward villain, reflecting the game's 
morally grey tone.

**The Player's Family** — The player's mother and younger sibling 
live in the target city. They are never seen but are a constant 
presence in the narrative, providing the emotional stakes.

### Game Elements

| Element              | Detail                                                    |
| -------------------- | --------------------------------------------------------- |
| Binary choices       | 32 total across four acts                                 |
| Game Over states     | 8 possible game over moments                              |
| Endings              | 3 — Win, Bittersweet, Lose                                |
| Scenario types       | Puzzle, Combat, Item Collection                           |
| Inventory categories | Weapons, Clothing, Consumables, Key Items                 |
| Score system         | Points awarded for combat, puzzles, and story progression |
| Save system          | Unlimited dossiers, one active tag at a time              |

**Three Endings:**

- **Win** — EISENFAUST is destroyed, the city is saved, the player 
  escapes. The player's real name is used in the final sequence.

- **Bittersweet** — EISENFAUST is destroyed and the city is saved, 
  but the player does not make it out. Inspired by le Carré's theme 
  of the spy as a disposable asset.

- **Lose** — The player's cover is blown, EISENFAUST fires, the city 
  is destroyed, and the player's family is gone.

### Choice Tree

The choice tree maps all 32 binary decisions across the four acts. 
Early choices carry forward consequences that shape later encounters. 
The tree converges in Act 4 where all paths lead to one of the three 
endings based on the cumulative weight of previous decisions.

![Choice Tree](../../assets/images/choice_tree.png)

---

## Sources of Inspiration

**John le Carré** — The morally ambiguous tone of Protocol Phantom 
is directly inspired by le Carré's spy fiction, particularly the 
portrayal of intelligence operatives as pawns of larger political 
forces rather than heroic figures. The bittersweet ending and the 
KGB Director's betrayal arc reflect themes found throughout his work, 
most notably in The Spy Who Came in from the Cold (1963), where 
loyalty and ideology are shown to be tools of manipulation rather 
than principles worth dying for.

**Schwerer Gustav Railway Gun** — The central threat in Protocol 
Phantom, the EISENFAUST railway cannon, is inspired by the real 
German Schwerer Gustav, an 80cm railway gun developed during World 
War II. At over 1,350 tonnes it remains one of the largest artillery 
pieces ever built. The scale and historical reality of such a weapon 
made it a compelling basis for the game's central threat.

**Personal interest in World War II** — A longstanding interest in 
WWII military history and documentaries provided the historical 
setting and technical details that ground the game's fictional 
narrative in a believable world. An earlier version of the game 
concept was set around the first German jet aircraft before the 
railway cannon was chosen as a more distinctive and dramatic threat.

---

## References

le Carré, J. (1963) *The Spy Who Came in from the Cold*. London: 
Victor Gollancz.

Hogg, I.V. (1998) *German Artillery of World War Two*. Mechanicsburg: 
Stackpole Books.

Stroustrup, B. (2013) *The C++ Programming Language*. 4th edn. 
Upper Saddle River: Addison-Wesley.
