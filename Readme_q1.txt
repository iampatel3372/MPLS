Compile the program as following:

g++ answer1.cpp -o routing

Running the Program :
Following Arguements needs to be Passed :
1).Topology File ex- top14.txt
2).Connection Request File ex- ARPANET_100.txt
3).Flag for chossing hop / path distance as metric ex- 0(for Path Distance) 1(For selecting Hop Distance)
4).-p / flag for choosing Optimistic / Pessimistic Approach (0 for Optimistic / 1 For Pessimistic)

Run like examples given Below :
./routing top14.txt NSFNET_100.txt 0 0 (Optmistic + Path Distance as Metric)
./routing top14.txt NSFNET_100.txt 1 0 (Optimistic + Hop Distance as Metric) 
./routing top24.txt ARPANET_100.txt 0 0
./routing top14.txt ARPANET_100.txt 1 0 

Output :
Results are Stored for each Tesing in Results.zip (for all Combinations of File)
Accepted Connection and Requests along with Blocking Probability is showed on Terminal.

