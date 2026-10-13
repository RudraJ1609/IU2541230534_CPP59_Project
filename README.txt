Rental Property Management  |  IU2541230534  |  CPP59

BUILD (from this folder):
  g++ -std=c++11 Source/*.cpp -o Executable/Project        (on Windows name it Project.exe)
RUN (from this folder, so the Data/ path is found):
  ./Executable/Project

Menu: 1 Add, 2 Display, 3 Search, 4 Update, 5 Delete, 6 Create Rental Agreement,
      7 Report, 8 Save/Load, 9 Exit (auto-saves).
Order to try: add Owner -> add Property -> add Tenant -> create Agreement -> Report.
Data files (Data/): owners.txt, tenants.txt, properties.txt, agreements.txt ('|' separated).
Diagrams (PlantUML text) are in Diagrams/. Paste into plantuml.com to render PNGs.
