#include <iostream>
#include <string>
using namespace std;

// Base class 
class Match {
public:
    void matchInfo() { cout << "Welcome to the Cricket Match!\n"; }
};

// Derived class 
class Scoreboard : public Match {
public:
    int totalRuns, wickets, overs;
    string striker, nonStriker, bowler;
    string battingTeamPlayers[11], bowlingTeamPlayers[11];
    int batsmanRuns[11], batsmanBalls[11], bowlerWickets[11], batsmanOut[11];
    int batsmanFours[11], batsmanSixes[11];
    int bowlerBalls[11];  
    int strikerIndex, bowlerIndex;
    int bowlerBowled[11];  
    
    // Constructor
    Scoreboard(string s1, string s2, string b, string batPlayers[], string bowlPlayers[]) {
        striker = s1; nonStriker = s2; bowler = b;
        totalRuns = wickets = overs = 0;
        for (int i = 0; i < 11; i++) {
            battingTeamPlayers[i] = batPlayers[i];
            bowlingTeamPlayers[i] = bowlPlayers[i];
            batsmanRuns[i] = batsmanBalls[i] = bowlerWickets[i] = batsmanOut[i] = 0;
            batsmanFours[i] = batsmanSixes[i] = bowlerBowled[i] = bowlerBalls[i] = 0;
        }
        strikerIndex = findPlayerIndex(battingTeamPlayers, striker);
        bowlerIndex = findPlayerIndex(bowlingTeamPlayers, bowler);
        bowlerBowled[bowlerIndex] = 1;
    }
    
    int findPlayerIndex(string arr[], string name) {
        for (int i = 0; i < 11; i++) if (arr[i] == name) return i;
        return -1;
    }
  //Polymorphism(Function Overloading)  
    void runs(int r) {
        if (r < 0 || r > 3) { 
            cout << "Invalid input! Only 0–3 allowed.\n";
            return; 
        }
        totalRuns += r;
        batsmanRuns[strikerIndex] += r; 
        batsmanBalls[strikerIndex]++;
        bowlerBalls[bowlerIndex]++;  
        if (r % 2 == 1) { 
            swap(striker, nonStriker); 
            strikerIndex = findPlayerIndex(battingTeamPlayers, striker); 
        }
        display();
    }
    
    void runs(int r, bool isBoundary) {
        if (r != 4 && r != 6) { 
            cout << "Invalid boundary input! Only 4 or 6.\n"; 
            return; 
        }
        totalRuns += r;
        batsmanRuns[strikerIndex] += r; 
        batsmanBalls[strikerIndex]++;
        bowlerBalls[bowlerIndex]++;  
        if (r == 4) batsmanFours[strikerIndex]++; 
        else batsmanSixes[strikerIndex]++;
        cout << striker << " hits a " << (r == 4 ? "FOUR!" : "SIX!") << "\n";
        display();
    }
    
    void wicket() {
        batsmanOut[strikerIndex] = 1; 
        batsmanBalls[strikerIndex]++;
        bowlerBalls[bowlerIndex]++; 
        bowlerWickets[bowlerIndex]++; 
        wickets++;
        cout << "WICKET! " << striker << " is out!\n";
        string nextBatsman; 
        int found = 0;
        while (true) {
            cout << "Enter next batsman: "; 
            cin >> nextBatsman;
            if (nextBatsman == nonStriker) { 
                cout << "Striker and non-striker cannot be same!\n"; 
                continue; 
            }
            for (int i = 0; i < 11; i++) {
                if (battingTeamPlayers[i] == nextBatsman) {
                    if (batsmanOut[i] == 1) { 
                        cout << "Already out!\n"; 
                        found = 0; 
                    } else { 
                        striker = nextBatsman; 
                        strikerIndex = i; 
                        found = 1; 
                    }
                    break;
                }
            }
            if (!found) 
                cout << "Invalid player! Enter again.\n";
            if (found) break;
        }
        display();
    }
    
    void noBall() { 
        totalRuns++; 
        cout << "No Ball! +1 run.\n"; 
        display(); 
    }
    
    void wideBall() {
        totalRuns++; 
        cout << "Wide Ball! +1 run.\n"; 
        display(); 
    }
    
    void nextOver() {
        overs++;
        swap(striker, nonStriker);
        strikerIndex = findPlayerIndex(battingTeamPlayers, striker);
        cout << "\nOver " << overs << " complete. Strike rotated.\n";
        display();
    }

    void changeBowler() {
        while (true) {
            cout << "Enter next bowler: "; 
            cin >> bowler;
            bowlerIndex = findPlayerIndex(bowlingTeamPlayers, bowler);
            if (bowlerIndex == -1) 
                cout << "Invalid bowler name!\n";
            else { 
                bowlerBowled[bowlerIndex] = 1; 
                break; 
            }
        }
    }

    void display() {
        cout << "-----------------------------\n";
        cout << "Score: " << totalRuns << "/" << wickets << " in " << overs << " over(s)\n";
        cout << "Striker: " << striker << " | Non-Striker: " << nonStriker << " | Bowler: " << bowler << "\n";
        cout << "-----------------------------\n";
    }
    
    
    double getStrikeRate(int runs, int balls) {
        if (balls == 0) return 0.0;
        return (static_cast<double>(runs) / balls) * 100;
    }

   
    double getEconomy(int runsConceded, int ballsBowled) {
        if (ballsBowled == 0) return 0.0;
        double oversBowled = ballsBowled / 6.0;
        return runsConceded / oversBowled;
    }

    void overview(string battingTeam, string bowlingTeam) {
        cout << "\n===== " << battingTeam << " INNINGS OVERVIEW =====\n";
        for (int i = 0; i < 11; i++)
            if (batsmanBalls[i] > 0 || batsmanRuns[i] > 0 || batsmanOut[i] == 1)
                cout << battingTeamPlayers[i] << ": " << batsmanRuns[i] << "(" << batsmanBalls[i] << ") "
                     << "4s:" << batsmanFours[i] << " 6s:" << batsmanSixes[i] 
                     << " SR:" << getStrikeRate(batsmanRuns[i], batsmanBalls[i])
                     << (batsmanOut[i] ? " [OUT]\n" : " [NOT OUT]\n");
        
        cout << "\n--- Bowling (" << bowlingTeam << ") ---\n";
        for (int i = 0; i < 11; i++) 
            if (bowlerBowled[i] == 1)
                cout << bowlingTeamPlayers[i] << ": " << bowlerWickets[i] << " wicket(s) "
                     << "Econ:" << getEconomy(totalRuns, bowlerBalls[i]) << "\n";  
    }
    
    int getRuns() { return totalRuns; }
    int getWickets() { return wickets; }
};
struct InningsResult {
    int runs, wickets;
    Scoreboard board;
    InningsResult(int r, int w, Scoreboard b) : runs(r), wickets(w), board(b) {}
};

InningsResult playInnings(string battingTeam, string bowlingTeam, string batPlayers[], string bowlPlayers[], int totalOvers, int target = -1) {
    string striker, nonStriker, bowler;
    cout << "\n--- " << battingTeam << " Innings ---\n";

    // check striker & non-striker validity
    while (true) {
        cout << "Enter striker: "; 
		cin >> striker;
        int found = 0;
        for (int i = 0; i < 11; i++) 
		if (batPlayers[i] == striker) 
		found = 1;
        if (!found) { 
		cout << "Invalid player! Not in batting team.\n"; 
		continue; }
        cout << "Enter non-striker: "; 
		cin >> nonStriker;
        if (nonStriker == striker) { 
		cout << "Striker and non-striker cannot be same!\n"; 
		continue; }
        found = 0;
        for (int i = 0; i < 11; i++) if (batPlayers[i] == nonStriker) found = 1;
        if (!found) { cout << "Invalid player! Not in batting team.\n"; continue; }
        break;
    }

    cout << "Enter bowler: "; cin >> bowler;
    Scoreboard board(striker, nonStriker, bowler, batPlayers, bowlPlayers);
    board.matchInfo();
    const int BALLS_PER_OVER = 6;
    for (int over = 1; over <= totalOvers; over++) {
        cout << "\n--- Over " << over << " ---\n";
        int ballCount = 0;
        while (ballCount < BALLS_PER_OVER) {
            cout << "Ball " << (ballCount + 1) << ": Enter result (0,1,2,3,4,6,W,N,D): ";
            char res; cin >> res;
            if (res >= '0' && res <= '6') {
                int r = res - '0';
                if (r == 4 || r == 6) board.runs(r, true); 
				else board.runs(r);
                ballCount++;
            } else if (res == 'W' || res == 'w') { 
			board.wicket(); 
			ballCount++; }
            else if (res == 'N' || res == 'n') 
			board.noBall();
            else if (res == 'D' || res == 'd') 
			board.wideBall();
            else cout << "Invalid input!\n";
            if (target != -1 && board.getRuns() > target) {
                cout << "\n" << battingTeam << " has chased the target!\n";
                return InningsResult(board.getRuns(), board.getWickets(), board);
            }
            if (board.getWickets() >= 10) break;
        }
        board.nextOver();
        if (over < totalOvers) board.changeBowler();
    }
    cout << "\nINNINGS COMPLETE for " << battingTeam << "!\n";
    return InningsResult(board.getRuns(), board.getWickets(), board);
}
int main() {
    cout << "It's Cricket Time!\n";
    string team1, team2;
    cout << "Enter Team 1: "; 
	cin >> team1;
    cout << "Enter Team 2: "; 
	cin >> team2;
    string team1Players[11], team2Players[11];
    cout << "\nEnter " << team1 << " players:\n";
    for (int i = 0; i < 11; i++) { 
	cout << "Player " << i + 1 << ": "; 
	cin >> team1Players[i]; }
    cout << "\nEnter " << team2 << " players:\n";
    for (int i = 0; i < 11; i++) { 
	cout << "Player " << i + 1 << ": "; 
	cin >> team2Players[i]; }
    string tossWinner, decision;
    cout << "\nWho won the toss (" << team1 << " or " << team2 << ")? "; 
	cin >> tossWinner;
    while (tossWinner != team1 && tossWinner != team2) { 
	cout << "Invalid team! Enter again: "; 
	cin >> tossWinner; }
    cout << "What did " << tossWinner << " choose (bat/bowl)? "; 
	cin >> decision;
    while (decision != "bat" && decision != "bowl") { 
	cout << "Invalid choice! Enter 'bat' or 'bowl': "; 
	cin >> decision; }
    int totalOvers; cout << "Enter total overs: "; 
	cin >> totalOvers;
    string battingFirst, bowlingFirst, battingSecond, bowlingSecond;
    if (decision == "bat") { 
	battingFirst = tossWinner; 
	bowlingFirst = (tossWinner == team1) ? team2 : team1; 
	}else { 
	bowlingFirst = tossWinner; 
	battingFirst = (tossWinner == team1) ? team2 : team1; 
	}
    battingSecond = bowlingFirst; 
	bowlingSecond = battingFirst;
    string *bat1 = (battingFirst == team1) ? team1Players : team2Players;
    string *bowl1 = (bowlingFirst == team1) ? team1Players : team2Players;
    string *bat2 = (battingSecond == team1) ? team1Players : team2Players;
    string *bowl2 = (bowlingSecond == team1) ? team1Players : team2Players;
    InningsResult first = playInnings(battingFirst, bowlingFirst, bat1, bowl1, totalOvers);
    cout << "\n" << battingFirst << " scored " << first.runs << "/" << first.wickets << ".\n";
    cout << "Target for " << battingSecond << ": " << first.runs + 1 << " runs.\n";
    InningsResult second = playInnings(battingSecond, bowlingSecond, bat2, bowl2, totalOvers, first.runs);
    cout << "\n--- MATCH RESULT ---\n";
    cout << battingFirst << ": " << first.runs << "/" << first.wickets << endl;
    cout << battingSecond << ": " << second.runs << "/" << second.wickets << endl;
    if (first.runs > second.runs) {
	cout << battingFirst << " WON by " << (first.runs - second.runs) << " runs.\n";
   } else if(second.runs > first.runs) {
   	cout << battingSecond << " WON by " << (10 - second.wickets) << " wickets.\n";
    }else{
	 cout << "Match TIED!\n";
}
    cout << "\n===== FULL MATCH SUMMARY =====\n";
    first.board.overview(battingFirst, bowlingFirst);
    second.board.overview(battingSecond, bowlingSecond);
    return 0;
}

