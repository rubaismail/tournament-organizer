/* COP 3502C Assignment 1
* This program is written by: Ruba Ismail
* UCF ID: 5186181
* Date: 1/21/2025
* Class: COP 3205C, Professor Tanvir
* Purpose: take in matches of teams and process the information
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 21

typedef struct Player {
   char *name; //Player name (dynamically allocated)
   int totalGoals; //Total goals scored by the player(needs to be counted based on the match)
}Player;


typedef struct Team {
   char *name; // Name of the team (dynamically allocated)
   Player **players; //An array of Player pointers where each pointer is pointing to a player
   int playerCount; // Number of players in the team
}Team;


typedef struct Goal {
   Player *scorer; // Pointer to the player who scored
   int time; // Time of the goal in minutes
}Goal;


typedef struct Match {
   Team *team1; // Pointer to Team 1
   Team *team2; // Pointer to Team 2
   Goal *goals; // Array of goals scored in the match
   int goalCount; // Total number of goals in the match
} Match;


//this is a special struct that should only be used to process a particular command
typedef struct MatchWinnerTeam{
   Team *winnerTeam; //pointer to the winner team if any
   int isDraw; //set to 1 if it was a draw. In that case winnerTeam will be NULL
}MatchWinnerTeam;


// dynamically allocates memory for a player struct
Player* createPlayer(char *name)
{
   Player *player = malloc(sizeof(Player));
   player->totalGoals = 0;

   int name_length = strlen(name) + 1;
   player->name = malloc(name_length * sizeof(char));
   strcpy(player->name, name);

   return player;
}

void freePlayer(Player *player){
   free(player->name);
   free(player);
}


// dynamically allocates memory for a Team struct
Team* createTeam(char *name, int playerCount)
{
   Team *team = malloc(sizeof(Team));
   team->playerCount = playerCount;

   int name_length = strlen(name) + 1;
   team->name = malloc(name_length * sizeof(char));
   strcpy(team->name, name);

   // creating an array of pointers that point to player structures (array of player pointers)
   team->players = malloc(playerCount * sizeof(Player *));

   for (int i = 0; i < playerCount; i++){
      team->players[i] = NULL;
   }

   return team;
}

void freeTeam(Team *team) {
   for (int i = 0; i < team->playerCount; i++) {
      freePlayer(team->players[i]);
   }
   free(team->players);
   free(team->name);
   free(team);
}


// iterates through a list of Player pointers and returns the player pointer based on the passed name
Player* getPlayerByName(char* name, Player** playerList, int numPlayers)
{
   for (int i = 0; i < numPlayers; i++)
   {
      if (strcmp(playerList[i]->name, name) == 0)
         return playerList[i];
   }
   return NULL;
}

// iterates through a list of Team pointers and returns the team pointer based on the passed name
Team* getTeamByName(char* name, Team** teamList, int numTeams)
{
   for (int i = 0; i < numTeams; i++)
   {
      if (strcmp(teamList[i]->name, name) == 0)
         return teamList[i];
   }
   return NULL;
}


// records a new goal for a match, adds the goal to the player's goal count
void recordGoal(Match *match, Player *player, int time)
{
   // reallocating space for an extra goal count
   match->goals = realloc(match->goals, (match->goalCount + 1) * sizeof(Goal));

   // accessing the new goal memory (the last in the goal array), adding the info from player
   match->goals[match->goalCount].time = time;
   match->goals[match->goalCount].scorer = player;

   player->totalGoals++;
   match->goalCount++;
}


// sums up the number of goals for each player and prints any players who scored a hattrick in the match
int printHatTrickPlayers( Match *match)
{
   // array of player pointers and int array of goals
   Player **pp = malloc(match->goalCount * sizeof(Player *));
   int *gc = calloc(match->goalCount, sizeof(int));

   int tracker = 0;

   for (int i = 0; i < match->goalCount; i++) {
      Player *scorer = match->goals[i].scorer;
      int found = 0;

      // search for scorer in pp
      for (int j = 0; j < tracker; j++)
      {
         if (pp[j] == scorer) {
            gc[j]++;                  // increment the gc goal count for the existing pp player
            found = 1;                // to mark player as found
            break;
         }
      }

      // scorer not in pp, adding the scorer to pp
      if (!found) {
         pp[tracker] = scorer;
         gc[tracker] = 1;
         tracker++;
      }
   }

   int hatFound = 0;
   // Prints header only once if a hat trick exists
   for (int i = 0; i < tracker; i++)
   {
      if (gc[i] >= 3) {
         if (!hatFound) {
            printf("hat trick found in %s vs %s:\n", match->team1->name, match->team2->name);
            hatFound = 1;    // Prevents further prints of the header
         }
         printf("%s %d\n", pp[i]->name, pp[i]->totalGoals);
      }
   }

   free(pp);
   free(gc);

   return hatFound;    // returns 0 if no hatrick, 1 if a hattrick was found
}


// finds which team the scorer of goals in the match are, returns team with most points in a MatchwinnerTeam struct
MatchWinnerTeam* getWinningTeam(Match *matches, int matchCount, char *team1Name, char *team2Name)
{
   MatchWinnerTeam *WT= malloc(sizeof(MatchWinnerTeam));
   WT->winnerTeam = NULL;
   WT->isDraw = 0;
   int matchFound = 0;

   for (int i = 0; i < matchCount; i++)
   {
      Match *m = &matches[i];
      if (strcmp(m->team1->name, team1Name) == 0 && strcmp(m->team2->name, team2Name) == 0)
      {
         matchFound = 1;
         int team1Goals = 0, team2Goals = 0;


         for (int j = 0; j < m->goalCount; j++)
         {
            Player *scorer = m->goals[j].scorer;

            for (int h = 0; h < m->team1->playerCount; h++) {
               if (m->team1->players[h] == scorer)
                  team1Goals++;
            }
            for (int h = 0; h < m->team2->playerCount; h++)
            {
               if (m->team2->players[h] == scorer)
                  team2Goals++;
            }
         }

         if (team1Goals > team2Goals)
            WT->winnerTeam = m->team1;
         else if (team2Goals > team1Goals)
            WT->winnerTeam = m->team2;
         else
            WT->isDraw = 1;
      }
   }
   if (!matchFound)
   {
      printf("no match between %s and %s\n", team1Name, team2Name);
      free(WT);
      return NULL;
   }
   return WT;
}


// finds the winning teams from a list of matches, awards points, team with most total points is the champion
Team* getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount)
{
   int *points = calloc(teamCount, sizeof(int));

   for (int i = 0; i < matchCount; i++)
   {
      MatchWinnerTeam *winner = getWinningTeam(matchList, matchCount,matchList[i].team1->name, matchList[i].team2->name);

      if (winner != NULL)
      {
         if (winner->isDraw == 1)       // if it's a draw, both teams get 1 point
         {
            for (int j = 0; j < teamCount; j++) {
               if (strcmp(teamList[j]->name, matchList[i].team1->name) == 0 || strcmp(teamList[j]->name, matchList[i].team2->name) == 0)
                  points[j] += 1;
            }
         }
         else                          // not a draw, finding the winning team
         {
            for (int j = 0; j < teamCount; j++)
            {
               if (strcmp(teamList[j]->name, winner->winnerTeam->name) == 0) {
                  points[j] += 3;
                  break;
               }
            }
         }
         free(winner);
      }
   }

   // finding team with the most points
   int maxPoints = -1;
   int Champ_index = 0;

   for (int i = 0; i < teamCount; i++)
   {
      if (points[i] > maxPoints) {
         maxPoints = points[i];
         Champ_index = i;
      }
   }

   free(points);
   return teamList[Champ_index];
}


void freeGoal(Goal *goal) {
   free(goal);
}

void freeMatch(Match *match, int numMatches) {

   for (int i = 0; i < numMatches; i++) {
        free(match[i].goals);
   }
   free(match);
}


int main(void)
{
   int numTeams = 0, numMatches = 0, numCommands = 0;
   scanf("%d%d%d", &numTeams, &numMatches, &numCommands);

   Player** playerList = NULL;
   Team** teamList = malloc( numTeams * sizeof(Team*));
   int playerCount = 0;

   for (int i = 0; i < numTeams; i++)
   {
      // Teams input
      char tempTeamName[MAX_LENGTH];
      int numPlayers = 0;
      scanf("%s%d", tempTeamName, &numPlayers);

      int Team_name_length = strlen(tempTeamName) + 1;
      char *teamName = malloc(Team_name_length * sizeof(char));
      strcpy(teamName, tempTeamName);

      teamList[i] = createTeam(teamName, numPlayers);
      free(teamName);


      for (int j = 0; j < numPlayers; j++)
      {
         // Players input
         char tempPlayerName[MAX_LENGTH];
         scanf("%s", tempPlayerName);

         int player_name_length = strlen(tempPlayerName) + 1;
         char *playerName = malloc( player_name_length * sizeof(char));
         strcpy(playerName, tempPlayerName);

         teamList[i]->players[j] = createPlayer(playerName);
         free(playerName);

         playerList = realloc(playerList, (playerCount + 1) * sizeof(Player*));
         playerList[playerCount] = teamList[i]->players[j];
         playerCount++;
      }
   }

    // Matches input
    Match *matchList = malloc(numMatches * sizeof(Match));
    for (int i = 0; i < numMatches; i++)
    {
        char tempTeam1Name[MAX_LENGTH], tempTeam2Name[MAX_LENGTH];
        scanf("%s %s", tempTeam1Name, tempTeam2Name);

        matchList[i].team1 = getTeamByName(tempTeam1Name, teamList, numTeams);
        matchList[i].team2 = getTeamByName(tempTeam2Name, teamList, numTeams);
        matchList[i].goalCount = 0;
        matchList[i].goals = NULL;

        char tempPlayerName[MAX_LENGTH];
        int goalTime;
        while (scanf("%s", tempPlayerName) && strcmp(tempPlayerName, "end") != 0)
        {
            scanf("%d", &goalTime);

            // Finding player in the global player list
            Player *scorer = getPlayerByName(tempPlayerName, playerList, playerCount);
            if (scorer != NULL)
                recordGoal(&matchList[i], scorer, goalTime);
        }
    }

    // Process commands
    for (int i = 0; i < numCommands; i++) {
        char command[MAX_LENGTH];
        scanf("%s", command);

       // player with max goals
        if (strcmp(command, "max_goal_player") == 0)
        {
            Player *maxPlayer = playerList[0];
            for (int j = 1; j < playerCount; j++) {
                if (playerList[j]->totalGoals > maxPlayer->totalGoals) {
                    maxPlayer = playerList[j];
                }
            }
            printf("player with max goal %s %d\n", maxPlayer->name, maxPlayer->totalGoals);

        }

       // winner of a match
        else if (strcmp(command, "winner") == 0)
        {
            char team1[MAX_LENGTH], team2[MAX_LENGTH];
            scanf("%s %s", team1, team2);
            MatchWinnerTeam *winner = getWinningTeam(matchList, numMatches, team1, team2);

           if (winner != NULL)
            {
                if (winner->isDraw)
                    printf("%s vs %s match drawn\n", team1, team2);
                else
                    printf("winner of %s vs %s is %s\n", team1, team2, winner->winnerTeam->name);

                free(winner);
            }
        }

       // hattrick
        else if (strcmp(command, "hattrick") == 0)
        {
           int hatTrickFound = 0;
           int found = 0;
           for (int j = 0; j < numMatches; j++)
           {
              found = printHatTrickPlayers(&matchList[j]);
              if (found)
                 hatTrickFound = 1;
           }
           if (hatTrickFound == 0)
              printf("no hat trick found in the tournament\n");
        }

       // champion of matches
        else if (strcmp(command, "champion") == 0)
        {
            Team *champion = getChampion(teamList, numTeams, matchList, numMatches);
            printf("champion is %s\n", champion->name);
        }
    }

   // Free all allocated memory
   for (int i = 0; i < numTeams; i++) {
      freeTeam(teamList[i]);
   }
   free(teamList);

   free(playerList);

   freeMatch(matchList, numMatches);
}