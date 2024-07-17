/**
 * @file model.h
 * @brief The declaration of all the data structures and functions related to the game in this MVC architecture.
 * @details The model is responsible for managing the internal information about the game, such as the boards, the boats and the attacks. All the information is inside the Game struct.
 * @authors Guilherme Matos, João Ferreira
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <lcom/lcf.h>

#define BOARD_LENGTH 15                         /// The length of the (square) board
#define BOARD_SIZE   BOARD_LENGTH*BOARD_LENGTH  /// The total number of squares in the board
#define BOAT_COUNT   8                          /// The number of boats in the game
#define SQUARE_COUNT (2+2+2+3+3+4+4+5)          /// The total number of squares occupied by the boats (sum of the sizes of all boats)

/**
 * @brief The structure representing a square in the board.
 * @details This usually is the return of the functions get_player_coord() and get_enemy_coord().
 * A Coord only represents a square of one of the boards (player or enemy).
 */
typedef struct {
  bool has_boat;  /// Whether the square has a boat
  bool is_hit;    /// Whether the square was hit or not
} Coord;

/**
 * @brief The structure representing a board.
 * @details This structure is used to represent the player or enemy board.
 * It is an single array of Coord structures, despite representing a matrix.
 * To access a specific square in the board, use the function get_player_coord() or get_enemy_coord().
 */
typedef struct {
  Coord board[BOARD_SIZE];  /// The array of Coord structures representing the board
} Board;

/**
 * @brief The types of boats that can be added to the board.
 * @details The number of squares of each boat is represented by the number of the enum.
 */
enum BoatType {
  carrier = 5,    /// Boat of 5 squares 
  battleship = 4, /// Boat of 4 squares
  submarine = 3,  /// Boat of 3 squares (also called cruiser)
  destroyer = 2,  /// Boat of 2 squares (also called patrol boat)
};

typedef struct {
  enum BoatType type;
  uint8_t x;
  uint8_t y;
  bool horizontal;
} Boat;

/**
 * @struct All the information about the state of the game.
 * @details This structure contains two boards:\n
 * - player: All the information about the player's boats and enemy hits\n
 * - enemy: Only the information about the player hits and the enemy boats already discovered by the player.\n
 * - boats: The array of boats of the player. Used for drawing the boats in the screen.\n
 * - boat_count: The number of boats already added to the player board.\n
 */
typedef struct {
  Board player; /// The board of the user playing the game
  Board enemy;  /// The (incomplete) board of the enemy
  Boat boats[BOAT_COUNT]; /// The array of boats of the player
  uint8_t boat_count; /// Boats already added to the board
} Game;

/**
 * @brief Initializes a new game struct.
 * @return A pointer to the new game struct.
 */
Game* (new_game)();

/**
 * @brief Deletes a game struct.
 * @warning Only use this function when the game ended or is not needed anymore. Use is_over() to check that.
 * @param game The game struct to be deleted.
 */
void (delete_game)(Game* game);

/**
 * @brief Get a pointer to a specific coordinate in a board (player or enemy).
 * @param board A pointer to the board struct.
 * @param x The column of the square.
 * @param y The row of the square.
 * @return A pointer to the Coord struct.
 */
Coord* (get_coord)(Board* board, uint8_t x, uint8_t y);

/**
 * @brief Add a boat to the player board.
 * @details This function also checks if the boat is out of bounds or if there is already a boat in the given position.
 * @param g The game struct.
 * @param type The type/size of the boat to be added.
 * @param x The column of upper left corner of the boat.
 * @param y The row of the upper left corner of the boat.
 * @param horizontal Whether the boat is horizontal or vertical.
 * @return Whether the boat was successfully added or if it failed in one of the checks.
 */
bool (add_boat)(Game* g, enum BoatType type, uint8_t x, uint8_t y, bool horizontal);

/**
 * @brief The player attacks a square in the enemy board.
 * @details Marks the square as hit in the enemy board and annotates the result. 
 * @param game The game struct.
 * @param x The column of the square to be attacked.
 * @param y The row of the square to be attacked.
 * @param has_boat Whether the square has a boat or not.
 */
void (attack)(Game* game, uint8_t x, uint8_t y, bool has_boat);

/**
 * @brief The enemy attacks a square in the player board.
 * @details Marks the square as hit in the player board and returns the result of the attack.
 * @param game The game struct.
 * @param x The column of the square to be attacked.
 * @param y The row of the square to be attacked.
 * @return Whether the attack has hit a boat or not.
 */
bool (receive)(Game* game, uint8_t x, uint8_t y);

/**
 * @brief Checks if a board has all its boats sunk.
 * @param board The board to be checked.
 * @return True if all boats are sunk, false otherwise.
 */
bool (is_over)(Board* board);

/**
 * @brief Print the game struct to the terminal.
 */
void debug_game(Game* game);


#endif /* _MODEL_H_ */
