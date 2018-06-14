#ifndef slic3r_GLGizmo_hpp_
#define slic3r_GLGizmo_hpp_

#include "../../slic3r/GUI/GLTexture.hpp"

namespace Slic3r {

class BoundingBoxf3;
class Pointf3;

namespace GUI {

class GLGizmoBase
{
protected:
    static const float BaseColor[3];
    static const float HighlightColor[3];
    static const float GrabberHalfSize;

public:
    enum EState
    {
        Off,
        Hover,
        On,
        Num_States
    };

protected:
    EState m_state;
    // textures are assumed to be square and all with the same size in pixels
    // no internal check is done
    GLTexture m_textures[Num_States];

public:
    GLGizmoBase();
    virtual ~GLGizmoBase();

    bool init();

    EState get_state() const;
    void set_state(EState state);

    unsigned int get_textures_id() const;
    int get_textures_size() const;

    void render(const BoundingBoxf3& box) const;

protected:
    virtual bool on_init() = 0;
    virtual void on_render(const BoundingBoxf3& box) const = 0;

    void _render_square(const Pointf3& center) const;
};

class GLGizmoRotate : public GLGizmoBase
{
    static const float Offset;
    static const unsigned int CircleResolution;
    static const unsigned int ScaleStepsCount;
    static const float ScaleStepRad;
    static const unsigned int ScaleLongEvery;
    static const float ScaleLongTooth;
    static const float ScaleShortTooth;
    static const unsigned int SnapRegionsCount;
    static const float GrabberOffset;

    float m_angle_x;
    float m_angle_y;
    float m_angle_z;

public:
    GLGizmoRotate();

protected:
    virtual bool on_init();
    virtual void on_render(const BoundingBoxf3& box) const;

private:
    void _render_circle(const Pointf3& center, float radius) const;
    void _render_scale(const Pointf3& center, float radius) const;
    void _render_snap_radii(const Pointf3& center, float radius) const;
    void _render_reference_radius(const Pointf3& center, float radius) const;
    void _render_grabber(const Pointf3& center, float radius) const;
};

class GLGizmoScale : public GLGizmoBase
{
    static const float Offset;

    float m_scale_x;
    float m_scale_y;
    float m_scale_z;

public:
    GLGizmoScale();

protected:
    virtual bool on_init();
    virtual void on_render(const BoundingBoxf3& box) const;
};

} // namespace GUI
} // namespace Slic3r

#endif // slic3r_GLGizmo_hpp_
