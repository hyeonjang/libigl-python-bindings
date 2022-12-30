#include <common.h>
#include <npe.h>
#include <typedefs.h>
#include <igl/direct_delta_mush.h>

const char* ds_direct_delta_mush = R"igl_Qu8mg5v7(
Perform Direct Delta Mush Skinning. 
Computes Direct Delta Mesh Skinning (Variant 0) from 
"Direct Delta Mush Skinning and Variants"

Parameters
----------
v  #V by 3 list of rest pose vertex positions
e  #E Number of bones
t  #E*4 by 3 list of bone pose transformations
omega #V by #E*10 list of precomputated matrix values

Returns
-------
u  #V by 3 list of output vertex positions

See also
--------


Notes
-----
None

Examples
--------
)igl_Qu8mg5v7";

npe_function(direct_delta_mush)
npe_doc(ds_direct_delta_mush)

npe_arg(v, dense_float, dense_double)
npe_arg(t, dense_float, dense_double)
npe_arg(omega, dense_float, dense_double)

npe_begin_code()
  assert_cols_equals(v, 3, "v");
  assert_valid_bone_transforms(t, "t");
  assert_rows_equals(t, (omega.cols() * 4) / 10, "t");

  Eigen::MatrixXd v_copy = v.template cast<double>();
  Eigen::MatrixXd t_copy = t.template cast<double>();
  Eigen::MatrixXd omega_copy = omega.template cast<double>();

  std::vector<Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d>> 
      t_affine(t_copy.rows() / 4);
  
  for(int bone = 0; bone < t_copy.rows(); bone += 4)
  {
    Eigen::Matrix4d t_bone;
    t_bone << t_copy.block(bone, 0, 4, 3).transpose(), 0.0, 0.0, 0.0, 0.0;
    Eigen::Affine3d a_bone;
    a_bone.matrix() = t_bone;

    t_affine[bone] = a_bone;
  }

  Eigen::MatrixXd u;
  igl::direct_delta_mush(v_copy, t_affine, omega_copy, u);
  return npe::move(u);

npe_end_code()

const char* ds_direct_delta_mush_precomp = R"igl_Qu8mg5v7(
Do the Omega precomputation necessary for Direct Delta Mush Skinning.

Parameters
----------
v  #V by 3 list of rest pose vertex positions
f  #F by 3 list of triangle indices into rows of V
w  #V by #Edges list of weights
p  number of smoothing iterations
lambda  rotation smoothing step size
kappa   translation smoothness step size
alpha   translation smoothness blending weight

Returns
-------
omega : #V by #E*10 list of precomputated matrix values

See also
--------


Notes
-----
None

Examples
--------
)igl_Qu8mg5v7";

npe_function(direct_delta_mush_precomputation)
npe_doc(ds_direct_delta_mush_precomp)

npe_arg(v, dense_float, dense_double)
npe_arg(f, dense_int, dense_long, dense_longlong)
npe_arg(w, npe_matches(v))
npe_arg(p, int)
npe_arg(lambda, double)
npe_arg(kappa, double)
npe_arg(alpha, double)

npe_begin_code()
  assert_valid_3d_tri_mesh(v, f);

  Eigen::MatrixXd v_copy = v.template cast<double>();
  Eigen::MatrixXd w_copy = w.template cast<double>();
  Eigen::MatrixXi f_copy = f.template cast<int>();

  Eigen::MatrixXd omega;
  igl::direct_delta_mush_precomputation(v_copy, f_copy, w_copy, p, lambda, kappa, alpha, omega);
  return npe::move(omega);
  
npe_end_code()
