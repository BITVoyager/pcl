/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2009-2010, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 *
 */

#ifndef PCL_SAMPLE_CONSENSUS_MODEL_NORMALPLANE_H_
#define PCL_SAMPLE_CONSENSUS_MODEL_NORMALPLANE_H_

#include <pcl/sample_consensus/sac_model.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/sample_consensus/sac_model_perpendicular_plane.h>
#include <pcl/sample_consensus/model_types.h>

namespace pcl
{
  /** \brief @b SampleConsensusModelNormalPlane defines a model for 3D plane
    * segmentation using additional surface normal constraints. Basically this
    * means that checking for inliers will not only involve a "distance to
    * model" criterion, but also an additional "maximum angular deviation"
    * between the plane's normal and the inlier points normals.
    *
    * The model coefficients are defined as:
    * <ul>
    * <li><b>a</b> : the X coordinate of the plane's normal (normalized)
    * <li><b>b</b> : the Y coordinate of the plane's normal (normalized)
    * <li><b>c</b> : the Z coordinate of the plane's normal (normalized)
    * <li><b>d</b> : the fourth <a href="http://mathworld.wolfram.com/HessianNormalForm.html">Hessian component</a> of the plane's equation
    * </ul>
    *
    * To set the influence of the surface normals in the inlier estimation
    * process, set the normal weight (0.0-1.0), e.g.:
    * \code
    * SampleConsensusModelNormalPlane<pcl::PointXYZ, pcl::Normal> sac_model;
    * ...
    * sac_model.setNormalDistanceWeight (0.1);
    * ...
    * \endcode
    *
    * \author Radu Bogdan Rusu and Jared Glover
    * \ingroup sample_consensus
    */
  template <typename PointT, typename PointNT>
  class SampleConsensusModelNormalPlane : public SampleConsensusModelPlane<PointT>, public SampleConsensusModelFromNormals<PointT, PointNT>
  {
    using SampleConsensusModel<PointT>::input_;
    using SampleConsensusModel<PointT>::indices_;
    using SampleConsensusModelFromNormals<PointT, PointNT>::normals_;
    using SampleConsensusModelFromNormals<PointT, PointNT>::normal_distance_weight_;

    public:

      typedef typename SampleConsensusModel<PointT>::PointCloud PointCloud;
      typedef typename SampleConsensusModel<PointT>::PointCloudPtr PointCloudPtr;
      typedef typename SampleConsensusModel<PointT>::PointCloudConstPtr PointCloudConstPtr;

      typedef typename SampleConsensusModelFromNormals<PointT, PointNT>::PointCloudNPtr PointCloudNPtr;
      typedef typename SampleConsensusModelFromNormals<PointT, PointNT>::PointCloudNConstPtr PointCloudNConstPtr;

      typedef boost::shared_ptr<SampleConsensusModelNormalPlane> Ptr;

      /** \brief Constructor for base SampleConsensusModelNormalPlane.
        * \param cloud the input point cloud dataset
        */
      SampleConsensusModelNormalPlane (const PointCloudConstPtr &cloud) : SampleConsensusModelPlane<PointT> (cloud)
      {
      }

      /** \brief Constructor for base SampleConsensusModelNormalPlane.
        * \param cloud the input point cloud dataset
        * \param indices a vector of point indices to be used from \a cloud
        */
      SampleConsensusModelNormalPlane (const PointCloudConstPtr &cloud, const std::vector<int> &indices) : SampleConsensusModelPlane<PointT> (cloud, indices)
      {
      }

      /** \brief Select all the points which respect the given model coefficients as inliers.
        * \param model_coefficients the coefficients of a plane model that we need to compute distances to
        * \param inliers the resultant model inliers
        * \param threshold a maximum admissible distance threshold for determining the inliers from the outliers
        */
      void 
      selectWithinDistance (const Eigen::VectorXf &model_coefficients, double threshold, std::vector<int> &inliers);

      /** \brief Compute all distances from the cloud data to a given plane model.
        * \param model_coefficients the coefficients of a plane model that we need to compute distances to
        * \param distances the resultant estimated distances
        */
      void 
      getDistancesToModel (const Eigen::VectorXf &model_coefficients, std::vector<double> &distances);

      /** \brief Return an unique id for this model (SACMODEL_NORMAL_PLANE). */
      inline pcl::SacModel 
      getModelType () const { return (SACMODEL_NORMAL_PLANE); }

    	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    protected:
      /** \brief Check whether a model is valid given the user constraints.
        * \param model_coefficients the set of model coefficients
        */
      bool 
      isModelValid (const Eigen::VectorXf &model_coefficients);

  };
}

#endif  //#ifndef PCL_SAMPLE_CONSENSUS_MODEL_NORMALPLANE_H_
