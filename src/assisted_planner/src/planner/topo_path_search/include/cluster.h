//
// Created by bottle on 2021/8/8.
//

#ifndef SRC_CLUSTER_H
#define SRC_CLUSTER_H

#pragma once
#include <vector>
#include <pcl/point_cloud.h>
#include <pcl/search/impl/search.hpp>
#include <pcl/io/pcd_io.h>
#include <pcl/search/search.h>
#include <pcl/kdtree/kdtree.h>
#include <chrono>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace std::chrono;


//struct clusters {
//
//};


//One object
class ClusterSave {
public:
    // ClusterSave(pcl::PointCloud<pcl::PointXYZ>::Ptr pc, boost::shared_ptr<std::vector<int>> indexPtr, pcl::PointXYZ& centroid)
    //     :m_avDensity(0), m_source(pc), m_index(indexPtr), m_centroid(centroid) {
    // }
    ClusterSave(pcl::PointCloud<pcl::PointXYZ>::Ptr pc, boost::shared_ptr<std::vector<int>> indexPtr, pcl::PointXYZ& centroid, boost::shared_ptr<std::vector<bool>> include)
            :m_avDensity(0), m_source(pc), m_index(indexPtr), m_centroid(centroid), m_include(include) {
    }
public:
    size_t size() const {
        return m_index->size();
    }
    pcl::PointXYZ& operator [] (int i) {
        return (*m_source)[(*m_index)[i]];
    }
    const std::vector<int>& inliner() {
        return *m_index;
    }
    pcl::PointXYZ& centroid() {
        return m_centroid;
    }
    bool check(int index) const{
        return (*m_include)[index];
    }
    void setBoundingBox(const float& minX, const float& maxX,const float& minY, const float& maxY, const float& minZ, const float& maxZ){
        m_minX = minX;
        m_maxX = maxX;
        m_minY = minY;
        m_maxY = maxY;
        m_minZ = minZ;
        m_maxZ = maxZ;
    }
    const float height(){
        // printf("height infomation, %f, %f, %f\n", m_maxZ, m_minZ, m_centroid.z);
        return max(m_maxZ - m_centroid.z, m_centroid.z - m_minZ);
    }
    const float width(){
        return max(m_maxY - m_centroid.y, m_centroid.y - m_minY);
    }
    const float length(){
        return max(m_maxX - m_centroid.x, m_centroid.x - m_minX);
    }
public:
    double m_avDensity;
    float m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
private:
    ClusterSave(ClusterSave&) {}
    ClusterSave& operator =(const ClusterSave&) {}
    ClusterSave& operator =(const ClusterSave&&) {}
public:
    boost::shared_ptr<std::vector<int>> m_index;
    pcl::PointXYZ m_centroid;
    pcl::PointCloud<pcl::PointXYZ>::Ptr m_source;
    boost::shared_ptr<std::vector<bool>> m_include;
};

typedef boost::shared_ptr<ClusterSave> ClusterSavePtr;
typedef std::vector<boost::shared_ptr<ClusterSave>> ClusterVector;
typedef boost::shared_ptr<std::vector<boost::shared_ptr<ClusterSave>>> ClusterVectorPtr;

class DBSCANClustering {
public:
    DBSCANClustering();
public:
    void setNum(double n);
    void setRadius(double r);
    void setObjectNum(int num);
    void setMargin(double m);
    void isAdaptive(bool flag);
    void setPushNum(double num);
    void cluster(pcl::PointCloud<pcl::PointXYZ>::Ptr source, ClusterVectorPtr midsave, ClusterVectorPtr unclusteredPtr, int& num);
    bool pointCloudCluster(pcl::PointCloud<pcl::PointXYZ> &cloud_in,
                                             std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> &cloud_out_vector,
                                             std::vector<Eigen::Vector3d> &cluster_central_vector);
private:
    void optimizationNum(const pcl::PointXYZ& p, double &r, double &n);
private:
    double adapt;
    double minNumInRadius;
    double radius;
    int minPointNumForOneObject;
    double scaleNumForIfPush;
    double margin;
};

#endif //SRC_CLUSTER_H
