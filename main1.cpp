#include<iostream>
#include<bits/stdc++.h>

using namespace std;

class path {
public:
	int src;
	int destination;
	int cost_path;
	vector<int> apath;
	path(int src, int destination, int cost_path, vector<int> paths)
	{
		this->src = src;
		this->destination = destination;
		this->cost_path = cost_path;
		for (int i = paths.size() - 1; i >= 0; i--)
		{
			apath.push_back(paths[i]);
		}
	}
};
class connects {
public:
	int src;
	int dest;
	int bmin;
	int bavg;
	int bmax;
	connects(int src, int dest, int bmin, int bavg, int bmax)
	{
		this->src = src;
		this->dest = dest;
		this->bmin = bmin;
		this->bavg = bavg;
		this->bmax = bmax;
	}
};
class forwarding_info {
public:
	int interface_in;
	int label_in;
	int interface_out;
	int label_out;
	forwarding_info(int interface_in, int label_in, int interface_out, int label_out)
	{
		this->interface_in = interface_in;
		this->label_in = label_in;
		this->interface_out = interface_out;
		this->label_out = label_out;
	}
};
class output_info {
public:
	int id;
	int src;
	int destination;
	int cost_path;
	vector<int> label;
	output_info(int id, int src, int destination, int cost_path, vector<int> la)
	{
		this->id = id;
		this->src = src;
		this->destination = destination;
		this->cost_path = cost_path;
		for (int i = 0; i < la.size(); i++)
		{
			label.push_back(la[i]);
		}
	}
};
int num_nodes;
int num_edges;
string srout="routing-";
string spath="path-";
string sforw="forwarding-";
string sout="Output-";
// Routing Vector holds Routing table
vector<path> routing;
// connections vector holds Connection Request Details
vector<connects> connections;
// forward table is Forwarding Table
vector<vector<forwarding_info>> forward_table;
// Output Vector holds Connection Requests succesfully accepted and their SOURCE DESTINATION and associated Labels
vector<output_info> output;
// used to maintain bandwidth associated with each edge of Graph
map<pair<int, int>, float> bandwidth;
// Interface / port information for each associated edge
map<pair<int, int>, int> interface;
// Total Request contains total connection Requests
int total_requests = 0;
// Total Connections Succesfully Established
int count_accept = 0;

int min_vertex(int* distance, bool * visited)
{
	int index = INT_MIN;
	for (int i = 0; i < num_nodes; i++)
	{
		if (!visited[i])
		{
			if (index == INT_MIN || distance[i] < distance[index])
			{
				index = i;
			}
		}
	}
	return index;
}

int djikstra(vector<vector<int>>&graph, int src, vector<int>&parent, int destination)
{
	vector<int>distance(num_nodes, INT_MAX);
	vector<bool>visited(num_nodes, false);
	distance[src] = 0, parent.resize(num_nodes, -1);
	for (int i = 0; i < num_nodes; i++)
	{
		int vertex = min_vertex(distance, visited);
		visited[vertex] = true;
		for (int j = 0; j < num_nodes; j++)
		{
			if (graph[vertex][j] && visited[j] == false)
			{
				int cur_distance = graph[vertex][j] + distance[vertex];
				if (cur_distance < distance[j])
				{
					parent[j] = vertex;
					distance[j] = cur_distance;
				}
			}
		}
	}
	int z = destination;
	vector<int> temp;
	temp.push_back(destination);
	while (parent[z] != -1)
	{
		temp.push_back(parent[z]);
		z = parent[z];
	}
	path p1(src, destination, distance[destination], temp);
	routing.push_back(p1);
	return distance[destination];
}


void create_table(vector<int> &path, vector<int> &label)
{
	int i = 0;
	for (; i < path.size() - 1; i++)
	{
		if (i == 0)
		{
			pair<int, int> p1(path[i], path[i + 1]);
			int interface_out = interface[p1];
			int label_out = rand() % 800;
			label.push_back(-1);
			label.push_back(label_out);
			forwarding_info f1(-1, -1, interface_out, label_out);
			forward_table[path[i]].push_back(f1);
		}
		else
		{
			int ind = forward_table[path[i - 1]].size() - 1;
			pair<int, int> p1(path[i], path[i - 1]);
			int interface_in = interface[p1];
			int label_in = forward_table[path[i - 1]][ind].label_out;
			pair<int, int> p2(path[i], path[i + 1]);
			int interface_out = interface[p2];
			int label_out = rand() % 1000;
			label.push_back(label_in);
			label.push_back(label_out);
			forwarding_info f1(interface_in, label_in, interface_out, label_out);
			forward_table[path[i]].push_back(f1);
		}
	}
	int ind = forward_table[path[i - 1]].size() - 1;
	pair<int, int> p1(path[i], path[i - 1]);
	int interface_in = interface[p1];
	int label_in = forward_table[path[i - 1]][ind].label_out;
	label.push_back(label_in);
	label.push_back(-1);
	forwarding_info f2(interface_in, label_in, -1, -1);
	forward_table[path[i]].push_back(f2);
}
// Find Index of entry in Routing table for Source and Destination associated with Connection Request
int find_index(int s, int d)
{
	for (int i = 0; i < routing.size(); i++)
	{
		if (routing[i].destination == d && routing[i].src == s)
		{
			return i;
		}
	}
	return -1;
}
// Create Connections as per optimistic / Pessimistic as specified by User
void create_connection(int approach)
{
	for (int i = 0; i < connections.size(); i++)
	{
		int test_source = connections[i].src;
		int test_destination = connections[i].dest;
		int index = find_index(test_source, test_destination);
		if(index == -1) continue;
		float bequiv_1 = 0;
		if (approach == 0)
		{
			float val = float(connections[i].bavg) + (0.25 * float((connections[i].bmax - connections[i].bmin)));
			bequiv_1 = min(float(connections[i].bmax), val);
		}
		else
		{
			bequiv_1 = float(connections[i].bmax);
		}
		int flag = 0;
		for (int j = 0; j < routing[index].apath.size() - 1; j++)
		{
			pair<int, int> p1(routing[index].apath[j], routing[index].apath[j + 1]);
			if (float(bandwidth[p1]) >= bequiv_1)
			{
				continue;
			}
			else
			{
				// 1st Path Cannot Fullfill Bandwidth Approach so 2nd Path is being Checked
				flag = -1;
				for (int l = 0; l < routing[index + 1].apath.size() - 1; l++)
				{
					pair<int, int> p1(routing[index + 1].apath[l], routing[index + 1].apath[l + 1]);
					if (float(bandwidth[p1]) >= bequiv_1)
					{
						continue;
					}
					// 2nd Path Also Failed Hence Connection Rejected
					else
					{
						flag = 1;
						break;
					}
				}
				break;
			}
		}
		// Flag = 0 means Connection Accepted using 1st Path
		if (flag == 0)
		{
			count_accept++;
			vector<int> label;
			create_table(routing[index].apath, label);
			output_info o1(i, test_source, test_destination, routing[index].cost_path, label);
			output.push_back(o1);
			for (int j = 0; j < routing[index].apath.size() - 1; j++)
			{
				pair<int, int> p1(routing[index].apath[j], routing[index].apath[j + 1]);
				pair<int, int> p2(routing[index].apath[j + 1], routing[index].apath[j]);
				bandwidth[p1] = bandwidth[p1] - bequiv_1;
				bandwidth[p2] = bandwidth[p2] - bequiv_1;
			}
		}
		// Flag = -1 means Connection Rejected using 2nd Path
		if (flag == -1)
		{
			count_accept++;
			vector<int> label;
			create_table(routing[index + 1].apath, label);
			output_info o1(i, test_source, test_destination, routing[index + 1].cost_path, label);
			output.push_back(o1);
			for (int j = 0; j < routing[index + 1].apath.size() - 1; j++)
			{
				pair<int, int> p1(routing[index + 1].apath[j], routing[index + 1].apath[j + 1]);
				pair<int, int> p2(routing[index + 1].apath[j + 1], routing[index + 1].apath[j]);
				bandwidth[p1] = bandwidth[p1] - bequiv_1;
				bandwidth[p2] = bandwidth[p2] - bequiv_1;
			}
		}
	}
}
// For writing to Output Files Appropriately
void writefiles()
{
	ofstream outputfile(srout, ios::out | ios::app | ios::ate);
	for (int i = 0; i < routing.size(); i++)
	{
		outputfile << "SRC : " << routing[i].src << " Destination : " << routing[i].destination << " Path : ";
		int j = 0;
		for (; j < routing[i].apath.size() - 1; j++)
		{
			outputfile << routing[i].apath[j] << " -> ";
		}
		outputfile << routing[i].apath[j] << "  Cost :" << routing[i].cost_path << endl;
	}
	outputfile.close();
	ofstream outputfile1(sforw, ios::out);
	for (int i = 0; i < forward_table.size(); i++)
	{
		outputfile1 << " Forwarding table  for Node " << i << endl;
		for (int j = 0; j < forward_table[i].size(); j++)
		{
			outputfile1 << "Label in : " << forward_table[i][j].label_in;
			outputfile1 << " Interface in : " << forward_table[i][j].interface_in;
			outputfile1 << " label out : " << forward_table[i][j].label_out;
			outputfile1 << " interface_out : " << forward_table[i][j].interface_out;
			outputfile1 << endl << endl;
		}
	}
	outputfile1.close();
	ofstream outputfile2(sout, ios::out);
	for (int i = 0; i < output.size(); i++)
	{
		outputfile2 << "ID " << output[i].id << " SRC " << output[i].src << " " << " Destination " << output[i].destination << "       Label { ";
		for (int j = 0; j < output[i].label.size(); j++)
		{
			outputfile2 << output[i].label[j] << " ";
		}
		outputfile2 << " } " << " Path Cost : " << output[i].cost_path << endl;
	}
	ofstream outputfile3(spath, ios::out);
	outputfile3 << "Requests " << connections.size() << " Accepted " << count_accept;
}

int main(int argc, char ** argv) {
	if(argc < 5) {
		cout << "Too few Command Line arguments are passed\n";
		cout << "Pass All arguements appropriately in Given Format : ./tin top14.txt NSFNET_100.txt 1(1 for chossing Hop as Distance) 1( for Pessimistic Approach)" << endl;
		return 0;
	}
	string s1 = argv[1], s2 = argv[2], s3 = argv[3], s4 = argv[4], s5 = s1 + "_" + s2 + "_" + s3 + "_" + s4 + ".txt";
	srout += s5;
	srand(time(0));

	ifstream file;
	file.open(s1);
	file >> num_nodes; file >> num_edges;
	vector<int>temp;
	int k;
	while(file >> k) temp.push_back(k);
	for(int i = 0; i < temp.size(); i += 4)
	file.close();
	vector<vector<int>>graph(num_nodes, vector<int>(num_nodes, 0));
	vector<vector<int>>graph1(num_nodes, vector<int>(num_nodes, 0));
	vector<int>parent(num_nodes, -1);
	int hop_as_distance = stoi(s3), approach = stoi(s4);
	if(hop_as_distance) {
		cout << "Hop as Distance metric is selected" << endl;
		for(int i = 0; i < temp.size(); i += 4) {
			int u = temp[i], v = temp[i+1];
			float band = (float) temp[i+3];
			bandwidth[make_pair(u,v)] = band;
			bandwidth[make_pair(v,u)] = band;
			graph[u][v] = graph[v][u] = graph1[u][v] = graph1[v][u] = 1;
		}
	} else {
		cout << "Cost as Distance metric is selected" << endl;
		for(int i = 0; i < temp.size(); i+=4) {
			int u = temp[i], v = temp[i+1];
			float band = (float) temp[i+3];
			bandwidth[make_pair(u,v)] = band;
			bandwidth[make_pair(v,u)] = band;
			graph[u][v] = graph[v][u] = graph1[u][v] = graph1[v][u] = temp[i+2];
		}
	}

	file.open(s2);
	vector<int>vconnect;
	file >> total_requests;
	int z;
	while(file >> z) vconnect.push_back(z);
	for (int i = 0; i < vconnect.size(); i += 5) {
		connects c1(vconnect[i], vconnect[i + 1], vconnect[i + 2], vconnect[i + 3], vconnect[i + 4]);
		connections.push_back(c1);
	}
	
	for(int i = 0; i < num_nodes; i++) {
		int count = 0;
		for(int j = 0; j < num_nodes; j++) {
			if(graph[i][j])
				interface[make_pair(i,j)]  = count++;
		}
	}
	for(int i = 0; i < num_nodes; i++) {
		vector<forwarding_info>v;
		forward_table.push_back(v);
	}

	for(int i = 0; i < num_nodes; i++) {
		for(int j = 0; j < num_nodes; j++) {
			if(i != j) {
				int cost_path1 = djikstra(graph, i, parent, j);
				int z = j;
				while (parent[z] != -1)
				{
					graph[z][parent[z]] = 0;
					graph[parent[z]][z] = 0;
					z = parent[z];
				}
				int cost_path2 = djikstra(graph, i, parent, j);
				for (int i = 0; i < num_nodes; i++)
				{
					for (int j = 0; j < num_nodes; j++)
					{
						graph[i][j] = graphf[i][j];
					}
				}
			}
		}
	}
	if(approach)
	{
		cout<<"Pessimistic Approach is Selected"<<endl;
	}
	else
	{
		cout<<"Optimistic Approach is Selected"<<endl;
	}
	create_connection(approach);
	writefiles();
	double probab=double((connections.size() - count_accept))/double(connections.size());
	cout<<"Number of Requests :"<<connections.size()<<" Accepted Connection " <<count_accept<<" Blocking Probability "<<probab<<endl;
	return 0;
}