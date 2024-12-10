V1 VERSION --> LOCATED IN "GAME" FILE

Console-Based Monopoly Game
Project Duration: Aug 2024 - Nov 2024
Project Overview
Developed a fully functional console-based implementation of the classic Monopoly board game using C++. This project demonstrates fundamental programming concepts and data structure implementation without using object-oriented programming.
Key Features

Multi-player support (2-4 players)
Complete property management system
Interactive trading system
Save/Load game functionality
Full implementation of classic Monopoly rules
Turn-based gameplay with dice mechanics
Property development (houses/hotels)
Bankruptcy and win condition handling

Technical Highlights

Implemented using pure C++ without object-oriented programming
Custom data structures using structs
Binary file handling for save/load functionality
Memory management and pointer manipulation
Complex game state management
Robust input validation and error handling
Modular code design with separate components

Skills Demonstrated

Advanced C++ Programming
Data Structure Design
File I/O Operations
Memory Management
Algorithm Implementation
Software Architecture
User Interface Design
Game Logic Implementation

Development Tools

C++
NetBeans IDE
Version Control (Git)
_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

MonopolyGame (V2 FINAL VERSION --> EVERYTHING LOCATED OUTSIDE OF GAME FOLDER)

MonopolyGame is a fully functional, object-oriented implementation of the classic Monopoly board game written in C++. This project is designed to demonstrate expertise in advanced programming concepts, including:

Classes: Modular and reusable components for game entities (e.g., Bank, Board, Player, Property).
Inheritance: Hierarchies like Property with derived classes (StandardProperty, RailroadProperty, UtilityProperty) and Space with specialized spaces (PropertySpace, TaxSpace, etc.).
Polymorphism: Abstract classes like Property and Space allow for flexible and dynamic behavior during gameplay.
Operator Overloading: Custom operators for Player (e.g., += and -= for money transactions).
Templates: Generalized resource management via the ResourceManager class.
Features
Core Game Logic:

Players can roll dice, buy/sell properties, manage resources, and handle game events like taxes and chance cards.
Implemented rules for property development, mortgaging, and bankruptcy.
Automated auctions and dynamic handling of player decisions.
Object-Oriented Design:

Highly modularized codebase, enabling easy extension and customization.
Clear separation of responsibilities among classes (e.g., Bank handles finances, Board manages spaces, Dice simulates randomness).
Resource Management:

Houses, hotels, and money are tracked and managed efficiently via the ResourceManager template.
Error Handling:

Robust exception handling (e.g., InsufficientFundsException, GameException) for a smooth user experience.
Game Flow:

Comprehensive turn-based system, including player actions like trading, developing properties, and exiting jail.
File Structure
main.cpp: Entry point for the application.
MonopolyGame.h / .cpp: Core game controller, managing the overall gameplay.
Board.h / .cpp: Handles board setup and movement across spaces.
Bank.h / .cpp: Manages game resources and property transactions.
Player.h / .cpp: Represents players, including their properties and finances.
Property.h / .cpp: Abstract base class for properties, with derived classes for Standard, Railroad, and Utility properties.
Dice.h / .cpp: Simulates dice rolls.
GameExceptions.h: Custom exception definitions.
ResourceManager.h: Template class for managing resources like houses, hotels, and money.
Technologies
C++: Core programming language.
Doxygen: For generating project documentation, including class diagrams and call graphs.
Graphviz: For visualizing UML diagrams and relationships.
Documentation
The project includes detailed documentation generated with Doxygen, including:

Class hierarchy
Call graphs
Collaboration diagrams
Inheritance structures
Run doxygen to regenerate documentation.

How to Run
Clone this repository.
Compile the project using a C++17-compatible compiler.
Run the executable.
Follow the game prompts to play Monopoly.
