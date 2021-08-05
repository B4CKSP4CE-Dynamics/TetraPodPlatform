#include <iostream>
#include <cmath>

#include <easy3d/viewer/viewer.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/renderer/drawable_lines.h>
#include <easy3d/renderer/drawable_points.h>
#include <easy3d/renderer/drawable_triangles.h>
#include <easy3d/renderer/vertex_array_object.h>
#include <easy3d/fileio/resources.h>
#include <easy3d/core/types.h>
#include <easy3d/util/logging.h>

#include "kinematics.hpp"

#define LOG(X) std::cerr << X << std::endl;

std::ostream& operator<<(std::ostream& os, Angles angles)
{
    return os << "j0: " << angles.j0 << ", j1: " << angles.j1 << ", j2: " << angles.j2;
}

int main1()
{
    Leg leg{1.0, 1.0, 1.0};
    LOG(leg_ik(leg, Vec3{1.0, 0.0, 1.0}));
    LOG(leg_ik(leg, Vec3{2.0, 0.0, 0.0}));
    LOG(leg_ik(leg, Vec3{1.0, 0.0, -1.0}));
    LOG(leg_ik(leg, Vec3{1.0 - sqrt(2) / 2, 0.0, sqrt(2) / 2}));
    return 0;
}

Leg const LEG{0.3, 0.5, 1};

Body const BODY{0.6, LEG};

easy3d::vec3 hor_rot_vec(real_t a)
{
    return {cos(a), sin(a), 0};
}

std::array<easy3d::vec3, 16> joint_positions(Body const& body, Actuations const& actuations)
{
    using easy3d::vec3;
    using easy3d::mat3;

    std::array<easy3d::vec3, 16> joints;

    auto adjust_angle = [](size_t i, real_t a) {
        return i == 1 || i == 2 ? M_PI - a : a;
    };

    auto coxa_vec = [&](auto a) {
        return hor_rot_vec(a) * body.leg.coxa_length;
    };

    joints[0] = vec3{body.side / 2, body.side / 2, 0};
    joints[4] = vec3(-body.side / 2, body.side / 2, 0 );
    joints[8] = vec3(-body.side / 2, -body.side / 2, 0 );
    joints[12] = vec3(body.side / 2, -body.side / 2, 0 );

    for (size_t i = 0; i < 4; ++i) {
        auto base_idx = i * 4;
        auto base_joint = joints[base_idx];
        auto angles = actuations.legs[i];

        auto rot = mat3::rotation(0 ,0, adjust_angle(i, angles.j0));

        vec3 pelvic{body.leg.coxa_length, 0.0, 0.0};
        vec3 knee = pelvic + mat3::rotation(0, -angles.j1, 0) * vec3{body.leg.femur_length, 0.0, 0.0};
        vec3 tip = knee + mat3::rotation(0, -angles.j2 - angles.j1, 0) * vec3{body.leg.tibia_length, 0.0, 0.0};
        joints[base_idx + 1] = base_joint + rot * pelvic;
        joints[base_idx + 2] = base_joint + rot * knee;
        joints[base_idx + 3] = base_joint + rot * tip;
    }

    return joints;
}

int main()
{
    using namespace easy3d;

    logging::initialize();

    Viewer viewer{"pavuk"};

    auto jarray = joint_positions(BODY, Actuations{Angles{M_PI / 4, M_PI / 4, -M_PI / 2}});
    auto joint_points = std::vector<vec3>(jarray.begin(), jarray.end());

    auto joints = new PointsDrawable("joints");
    joints->update_vertex_buffer(joint_points, true);
    joints->set_uniform_coloring(vec4(1.0, 0.0, 0.0, 1.0));
    joints->set_impostor_type(PointsDrawable::SPHERE);
    joints->set_point_size(20);
    viewer.add_drawable(joints);

    auto frame = new LinesDrawable("frame");
    frame->update_vertex_buffer(joint_points, true);
    frame->set_uniform_coloring(vec4(1.0, 0.0, 0.0, 1.0));
    std::vector<unsigned> indices{
        0, 1, 1, 2, 2, 3,
        4, 5, 5, 6, 6, 7,
        8, 9, 9, 10, 10, 11,
        12, 13, 13, 14, 14, 15,
        0, 4, 4, 8, 8, 12, 12, 0
    };
    frame->update_element_buffer(indices);
    viewer.add_drawable(frame);

    viewer.fit_screen();
    viewer.set_animation(true);

    vec3* joint_vertices = reinterpret_cast<vec3*>(VertexArrayObject::map_buffer(GL_ARRAY_BUFFER, joints->vertex_buffer(), GL_WRITE_ONLY));
    vec3* frame_vertices = reinterpret_cast<vec3*>(VertexArrayObject::map_buffer(GL_ARRAY_BUFFER, frame->vertex_buffer(), GL_WRITE_ONLY));

    float d = 0;

    viewer.animation_func_ = [&](Viewer& v) -> bool {
        auto jarray = joint_positions(BODY, Actuations{Angles{M_PI / 4, M_PI / 4, -M_PI / 2}});
        for (size_t i = 0; i < jarray.size(); ++i) {
            joint_vertices[i] = jarray[i];
            frame_vertices[i] = jarray[i];
        }

        v.update();
        return true;
    };

    return viewer.run();
}