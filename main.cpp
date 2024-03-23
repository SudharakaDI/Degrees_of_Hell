#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "CSpace.h"
#include "CPlayer.h"
#include "CAssessment.h"
#include <sstream>

void ReadSpaces(std::vector<CSpace*>& spaces, const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (file.is_open()) {
            int type;
            std::string name;
            int cost, achievement, year;
            while (file >> type) {
                std::getline(file >> std::ws, name); // Read the rest of the line as name
                if (type == 1) {
                    std::istringstream iss(name);
                    iss >> name >> cost >> achievement >> year;
                    CAssessment* assessment = new CAssessment(type, name, cost, achievement);
                    spaces.push_back(assessment);
                } else {
                    CSpace* space = new CSpace(type, name);
                    spaces.push_back(space);
                }
            }
            file.close();
        } else {
            throw std::runtime_error("Unable to open file: " + filename);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    // Read spaces from file
    std::vector<CSpace*> spaces;
    ReadSpaces(spaces, "degrees.txt");

    // Create players
    CPlayer player1("Vyvyan");
    CPlayer player2("Rick");

    // Welcome message
    std::cout << "Welcome to Scumbag College" << std::endl;

    // Simulate 20 rounds
    for (int round = 1; round <= 20; ++round) {
        // Display round number
        std::cout << "Round " << round << std::endl;

        // Player 1 turn
        player1.Spin();
        CSpace* currentSpace1 = spaces[player1.GetPosition()];
        std::cout << player1.GetName() << " lands on " << currentSpace1->GetName() << std::endl;
        if (CAssessment* assessment = dynamic_cast<CAssessment*>(currentSpace1)) {
            if (!assessment->IsCompleted()) {
                if (player1.GetMotivation() >= assessment->GetMotivationalCost()) {
                    player1.IncreaseMotivation(-assessment->GetMotivationalCost());
                    player1.IncreaseSuccess(assessment->GetAchievement());
                    assessment->SetCompleted(true);
                    std::cout << player1.GetName() << " completes " << currentSpace1->GetName()
                              << " for " << assessment->GetMotivationalCost()
                              << " and achieves " << assessment->GetAchievement() << std::endl;
                } else {
                    // Ask for help
                    std::cout << player1.GetName() << " asks for help with " << currentSpace1->GetName() << std::endl;
                    int helpCost = assessment->GetMotivationalCost() / 2;
                    int helpAchievement = assessment->GetAchievement() / 2;
                    if (player2.GetMotivation() >= helpCost) {
                        player1.IncreaseMotivation(-helpCost);
                        player2.IncreaseMotivation(-helpCost);
                        player1.IncreaseSuccess(helpAchievement);
                        player2.IncreaseSuccess(helpAchievement);
                        std::cout << player2.GetName() << " helps and achieves " << helpAchievement << std::endl;
                    } else {
                        std::cout << player2.GetName() << " cannot help due to low motivation" << std::endl;
                    }
                }
            } else {
                std::cout << currentSpace1->GetName() << " has already been completed" << std::endl;
            }
        } else {
            std::cout << "Nothing happens" << std::endl;
        }

        // Output player's motivation and success
        std::cout << player1.GetName() << "'s motivation is " << player1.GetMotivation()
                  << " and success is " << player1.GetSuccess() << std::endl;

        // Player 2 turn
        // Simulate player 2 turn similarly

    }

    // Game Over
    std::cout << "Game Over" << std::endl;
    std::cout << player1.GetName() << " has achieved " << player1.GetSuccess() << std::endl;
    std::cout << player2.GetName() << " has achieved " << player2.GetSuccess() << std::endl;
    if (player1.GetSuccess() > player2.GetSuccess()) {
        std::cout << player1.GetName() << " wins." << std::endl;
    } else if (player1.GetSuccess() < player2.GetSuccess()) {
        std::cout << player2.GetName() << " wins." << std::endl;
    } else {
        std::cout << "It's a tie." << std::endl;
    }

    // Clean up dynamically allocated memory
    for (auto space : spaces) {
        delete space;
    }

    return 0;
}
