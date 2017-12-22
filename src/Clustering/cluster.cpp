// Includes, project.
#include "cluster.hpp"

/*
 * points : Points d'entrée
 * clusters : tableau de cluster en sortie de l'algorithme
 */
void
sy31::computeClusters(std::vector<sy31::Cluster>& clusters, std::vector<sy31::vec2d> const& points)
{
  /* Exemple d'utilisation des clusters
   * clusters.clear();		// Effacer tout les clusters
   * cluster.resize(1);		// Créer un cluster
   * 
   * clusers[0].clear();	// Effacer le cluster 0 (Initialiser)
   * cluster[0].push_back(points[i]);	// Ajouter le point i au cluster 0
   $*/
	//int n = points.size();
	clusters.resize(1);
	clusters.clear();
	int k = 5;
	/*
	clusters.resize(1);
	clusters[0].clear();
	for(int i = 0; i<680; i++)
		clusters[0].push_back(points[i]);
	*/

	double d[680];
	double min =0;
	int nmin = 0;
	int g=0;
	int G[680];
	for(int i=0;i<680;i++){
		G[i] = 0;
	}
	for(int i =k; i<680; i++){
		if(points[i].x*points[i].x + points[i].y*points[i].y > 1){
			d[1] = (points[i].x - points[i-1].x)*(points[i].x - points[i-1].x) + (points[i].y - points[i-1].y)*(points[i].y - points[i-1].y);
			min = d[1];
			nmin = 1;
			for(int j = 2; j <=k; j++){
				d[j] = (points[i].x - points[i-j].x)*(points[i].x - points[i-j].x) + (points[i].y - points[i-j].y)*(points[i].y - points[i-j].y);
				if(d[j] < min) {
					min=d[j];
					nmin = j;
				}
			}
			if(min < 10000){
				if(G[i - nmin] == 0){
					clusters.resize(clusters.size()+1);
					clusters[g].push_back(points[i-nmin]);
					g = g+1;
					G[i-nmin] = g;
				}
				clusters[G[i-nmin]-1].push_back(points[i]);
			}
		}
	}
}
