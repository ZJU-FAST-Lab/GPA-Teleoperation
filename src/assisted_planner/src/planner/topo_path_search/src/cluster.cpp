//
// Created by bottle on 2021/8/8.
//
#include "cluster.h"
#include <stack>
#include <boost/make_shared.hpp>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_cloud.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <ctime>

DBSCANClustering::DBSCANClustering() {
    radius = 0.5;
    minNumInRadius = 0;
    adapt = false;
    scaleNumForIfPush = 1.0;
    minPointNumForOneObject = 9;
    margin = 0.3;
}
void DBSCANClustering::setRadius(double r) {
    radius = r;
}

void DBSCANClustering::setNum(double n) {
    minNumInRadius = n;
}

void DBSCANClustering::setObjectNum(int num){
    minPointNumForOneObject = num;
}

void DBSCANClustering::isAdaptive(bool flag) {
    adapt = flag;
}

void DBSCANClustering::setPushNum(double num) {
    scaleNumForIfPush = num;
}

void DBSCANClustering::setMargin(double m) {
    margin = m;
}

void DBSCANClustering::cluster(pcl::PointCloud<pcl::PointXYZ>::Ptr source, ClusterVectorPtr midsave, ClusterVectorPtr unclusteredPtr, int& filterNum) {
    filterNum = 0;
    if(source->size() == 0)
        return;

    //build kdtree
    boost::shared_ptr<pcl::KdTreeFLANN<pcl::PointXYZ>> kdtree = boost::make_shared<pcl::KdTreeFLANN<pcl::PointXYZ>>();
    kdtree->setInputCloud(source);

    //mark buffer
    std::vector<bool> pcindex(source->size(), false);
    //for every point
    for (int i = 0; i < source->size(); i++) {
        if (pcindex[i])
            continue;
        float m_minX=1e6, m_maxX=-1e6, m_minY=1e6, m_maxY=-1e6, m_minZ=1e6, m_maxZ=-1e6;

        boost::shared_ptr<std::vector<int>> category = boost::make_shared<std::vector<int>>();
        boost::shared_ptr<std::vector<bool>> include = boost::make_shared<std::vector<bool>>(source->size(), false);
        pcl::PointXYZ centroid;
        std::stack<int> stc;
        stc.push(i);
        while (true) {
            if (stc.empty()) {
                break;
            }
            std::vector<int> indices;
            std::vector<float> distance;
            optimizationNum(source->points[stc.top()], radius, minNumInRadius);

            //kdtree search
            kdtree->radiusSearch(source->points[stc.top()], radius, indices, distance);
            stc.pop();
            //enough point cloud size filter4
            if (indices.size() > minNumInRadius) {
                for (int i = 0; i < indices.size(); i++) {
                    if (!pcindex[indices[i]]) {
                        //mark it
                        pcindex[indices[i]] = true;
                        category->push_back(indices[i]);
                        (*include)[indices[i]] = true;
                        const pcl::PointXYZ& p = source->points[indices[i]];
                        m_minX = min(m_minX, p.x);
                        m_minY = min(m_minY, p.y);
                        m_minZ = min(m_minZ, p.z);

                        m_maxX = max(m_maxX, p.x);
                        m_maxY = max(m_maxY, p.y);
                        m_maxZ = max(m_maxZ, p.z);
                        centroid.getArray3fMap() = centroid.getArray3fMap() +p.getArray3fMap();
                        //if expand it
                        //size requirment
                        if (indices.size() > scaleNumForIfPush * minNumInRadius) {
                            //distance requirment
                            if (distance[i] > radius * margin) {
                                stc.push(indices[i]);
                            }
                        }
                    }
                }
            }
        }
        if (category->size() > minPointNumForOneObject) {
            centroid.getArray3fMap() = centroid.getArray3fMap() / category->size();
            boost::shared_ptr<ClusterSave> cs = boost::make_shared<ClusterSave>(source, category, centroid, include);
            cs->setBoundingBox(m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ);
            filterNum += category->size();
            midsave->push_back(cs);
        }
//        else if (category->size() > minPointNumForOneObject/4) {
//            centroid.getArray3fMap() = centroid.getArray3fMap() / category->size();
//            boost::shared_ptr<ClusterSave> cs = boost::make_shared<ClusterSave>(source, category, centroid, include);
//            cs->setBoundingBox(m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ);
//            filterNum += category->size();
//            unclusteredPtr->push_back(cs);
//        }
    }
}
void DBSCANClustering::optimizationNum(const pcl::PointXYZ& p, double &r, double &n) {
    if (adapt) {
        double distance = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
        double number = pow(10.0, 2.84351205 - 1.001837*log10(distance));
        n = number;
    }
}

// cluster
// output: cluster point cloud; cluster central point
bool DBSCANClustering::pointCloudCluster(pcl::PointCloud<pcl::PointXYZ> &cloud_in,
                                        std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> &cloud_out_vector,
                                        std::vector<Eigen::Vector3d> &cluster_central_vector)
{
    pcl::PointCloud<pcl::PointXYZ> temp_cloud = cloud_in;
    pcl::PointCloud<pcl::PointXYZ>::Ptr temp_cloud_ptr = temp_cloud.makeShared();

    pcl::search::KdTree<pcl::PointXYZ>::Ptr temp_cloud_kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
    temp_cloud_kdtree->setInputCloud(temp_cloud_ptr);

    pcl::EuclideanClusterExtraction<pcl::PointXYZ> cluster_extraction;
    cluster_extraction.setClusterTolerance(0.3);
    cluster_extraction.setMinClusterSize(0);
    cluster_extraction.setMaxClusterSize(10);
    cluster_extraction.setSearchMethod(temp_cloud_kdtree);
    cluster_extraction.setInputCloud(temp_cloud_ptr);
    std::vector<pcl::PointIndices> clusters;
    cluster_extraction.extract(clusters);

    bool cluster_success = false;
    int cluster_num = 0;
    int point_cloud_size = 0;
    cout<<clusters.size()<<endl;
    for (auto i = clusters.begin(); i != clusters.end(); ++i)
    {
//        cluster_num++;
//        pcl::PointCloud<pcl::PointXYZI>::Ptr current_cluster(new pcl::PointCloud<pcl::PointXYZI>);
//        // if the position change too large, this cluster is wrong
//        Eigen::Vector3d temp_car_point;
//        Eigen::Vector3d temp_current_car_p{0,0,0};
//        pcl::PointXYZI temp_point;
//        for (std::vector<int>::const_iterator point = i->indices.begin(); point != i->indices.end(); point++)
//        {
//            temp_car_point[0] = temp_cloud_ptr->points[*point].x;
//            temp_car_point[1] = temp_cloud_ptr->points[*point].y;
//            temp_car_point[2] = temp_cloud_ptr->points[*point].z;
//            current_cluster->points.push_back(temp_cloud_ptr->points[*point]);
//            temp_current_car_p = temp_current_car_p + temp_car_point;
//        }
//        temp_current_car_p = temp_current_car_p/current_cluster->points.size();
//
//        cluster_success = true;
//        cloud_out_vector.push_back(current_cluster);
//        cluster_central_vector.push_back(temp_current_car_p);
    }

    return cluster_success;
}



