#include "find_path.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "structure.h"
#include "m1.h"
#include "m3.h"
#include "m2.h"
#include "m2_helper.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <queue>

/*vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty)
{


    newMap.interM3[intersect_id_start].cost = 0;
    priority_queue<Intersection, vector<Intersection>, mycomp> queue;
    vector<unsigned> bestPath;
    vector<unsigned> bestPathSeg;
    queue.push(newMap.interM3[intersect_id_start]);

    while (!queue.empty())
    {
        Intersection leastCostNode = queue.top();
        queue.pop();

        if (leastCostNode.interId == intersect_id_end)
        { //if found path
            unsigned temp = intersect_id_end;
            while (temp != intersect_id_start)
            {
                bestPath.push_back(newMap.interM3[temp].through);
                temp = newMap.interM3[temp].from;
            }

            for (int pathCounter = bestPath.size() - 1; pathCounter >= 0; pathCounter--)
            {
                bestPathSeg.push_back(bestPath[pathCounter]);
            }

            return bestPathSeg;
        }

        if (leastCostNode.cost <= newMap.interM3[leastCostNode.interId].cost)
        {
            for (unsigned connectedCounter = 0; connectedCounter < leastCostNode.connected_inter_directed.size(); connectedCounter++)
            {
                unsigned to = leastCostNode.connected_inter_directed[connectedCounter];
                double tempCost = leastCostNode.cost + newMap.segtravelTime[leastCostNode.connected_streetSeg_directed[connectedCounter]];
                if (newMap.streetSegInfo[leastCostNode.through].inStreet != newMap.streetSegInfo[leastCostNode.connected_streetSeg_directed[connectedCounter]].inStreet)
                {
                    if (leastCostNode.interId != intersect_id_start)
                    {
                        tempCost += turn_penalty;
                    }
                }
                if (newMap.interM3[to].cost > tempCost)
                {
                    newMap.interM3[to].cost = tempCost;
                    newMap.interM3[to].from = leastCostNode.interId;
                    newMap.interM3[to].through = leastCostNode.connected_streetSeg_directed[connectedCounter];
                    queue.push(newMap.interM3[to]);
                }
            }
        }
    }
    return bestPathSeg;
}*/


/*std::vector<unsigned> AStarSearch(unsigned source, unsigned dest) {
    std::vector<unsigned> path;
    // check if both source and dest exists, if not, return empty vector
    // also check if clicked on two identical intersections
    unsigned maxValue = getNumberOfIntersections();
    if (source == dest || source >= maxValue || dest >= maxValue
            || source < 0 || dest < 0) {
        return path;
    }

    // reset gScores, hScores, visited flags, and arrivedFrom IDs
    cleanGraph();

    // priority Q sorted by time, with associated data bit intersectionID
    typedef std::pair<double, unsigned> NodePair;
    // declare an open set
    std::priority_queue< NodePair, std::vector<NodePair>, greater<NodePair> > openSet;

    // set up values for the source node
    intersection_graph[source].gScore = 0;
    intersection_graph[source].hScore = (findConsistentH(source, dest));
    //interGraph[source].setH(0);
    intersection_graph[source].fScore = (intersection_graph[source].getH());

    // add current to the open set
    NodePair insertToOpen(intersection_graph[source].fScore, source);
    openSet.push(insertToOpen);

    unsigned currentID = source;
    // declare a pointer to the current node under operation inside the path loop
    GNode* currentNode;
    currentNode = &(intersection_graph[source]);

    bool found = false;
    // explore the interGraph and updates the open sets
    while (!openSet.empty() && !found) {
        // mark the node with smallest F in open set to current
        currentID = openSet.top().second;
        if (currentID == dest) { // we found a path
            found = true;
            break;
        }

        // mark current as visited and remove it from open set
        currentNode = &(intersection_graph[currentID]);
        currentNode->arrivedFrom = (true);
        openSet.pop();

        // add neighbours of current to open set
        std::vector<InterPath> neighbours = currentNode->connections;
        for (int i = 0; i < (int) neighbours.size(); i++) {
            unsigned interID = neighbours[i].getInter();
            GNode* thisNode;
            thisNode = &(intersection_graph[interID]);

            // if already in closed set (visited), pass
            if (!thisNode->getVisited()) {

                // update G score (from source to here)
                double tentativeG = currentNode->getG() + neighbours[i].getDT();
                StreetSegmentInfo secondSeg = getStreetSegmentInfo(currentNode->getArrivedFrom());
                if (neighbours[i].getStreet() != secondSeg.streetID && currentNode->getID() != source)
                    // 15 seconds = 0.25 minutes
                    tentativeG += 0.25;
                // update H score (approx. to end)
                double tentativeH = findConsistentH(interID, dest);
                //double tentativeH = 0;
                // postulate F score using G + H
                double tentativeF = tentativeG + tentativeH;

                // try to update: 
                // case 1: fScore is infinite since thisNode not in the open set, add it
                // case 2: thisNode already on openSet, but this traversal might be better
                if (tentativeF < thisNode->getF()) {
                    //if (thisNode->getF() == numeric_limits<double>::infinity() || ) {
                    thisNode->setG(tentativeG);
                    thisNode->setH(tentativeH);
                    // update F score (G + H)
                    thisNode->setF(tentativeF);
                    // update arrival from
                    thisNode->setArrivedFrom(neighbours[i].getSeg());

                    // shove into open set
                    NodePair insertToOpen(tentativeF, interID);
                    openSet.push(insertToOpen);
                }// end of case 1
            }
            // detach thisNode ptr
            thisNode = NULL;
        } // end of neighbours
    }
    //detach currentNode pointer
    currentNode = NULL;
    // once dest is found, retrace the path back to start
    if (found) path = retrace(source, dest);

    return path;
}*/
