cmake_minimum_required(VERSION 3.0.0)
project(mt2-glut VERSION 0.1.0 LANGUAGES C)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

add_executable(glutBasics glutBasics.c
            trackball.c
			FistBlue/coinage.c
			FistBlue/collision.c
			FistBlue/coll_bonus.c
			FistBlue/coll_projectile.c
			FistBlue/computer.c
			FistBlue/demo.c
			FistBlue/effects.c
			FistBlue/endings.c
			FistBlue/fightgfx.c
			FistBlue/game.c
			FistBlue/gemu.c
			FistBlue/gfx_glut.c
			FistBlue/gfxlib.c
			FistBlue/scrolls/gstate.c
			FistBlue/scrolls/parallax.c
			FistBlue/scrolls/scroll_data.c
			FistBlue/scrolls/scroll_maint.c
			FistBlue/lib.c
			FistBlue/particle.c
			FistBlue/player.c
			FistBlue/playerselect.c
			FistBlue/playerstate.c
			FistBlue/projectiles.c
			FistBlue/reactmode.c
			FistBlue/rules.c
			FistBlue/sm.c
			FistBlue/sound.c
			FistBlue/sprite.c
			FistBlue/task.c
			FistBlue/text.c
			FistBlue/ai/ai.c
			FistBlue/tests/aitests.c
			FistBlue/tests/testlib.c
			RedHammer/pthreads.c
			RedHammer/redhammer.c
			SwiftBeam/glwimp.c
			FistBlue/actions/actions.c
			FistBlue/actions/actions_198a.c
			FistBlue/actions/actions_530a.c
			FistBlue/actions/act02_bicycleriders.c
			FistBlue/actions/act07_elephants.c
			FistBlue/actions/act16.c
			FistBlue/actions/act17.c
			FistBlue/actions/act1e_worldflags.c
			FistBlue/actions/act29_wwlogo.c
			FistBlue/actions/act2e_plane.c
			FistBlue/actions/act3e_capcomlogos.c
			FistBlue/actions/act_3f.c
			FistBlue/actions/car.c
			FistBlue/actions/drums.c
			FistBlue/actions/reels.c
			FistBlue/actions/barrels.c
			FistBlue/avatars/ryu.c
			FistBlue/avatars/guile.c
			FistBlue/avatars/ehonda.c
			FistBlue/avatars/blanka.c
			FistBlue/avatars/chunli.c
			FistBlue/avatars/zangeif.c
			FistBlue/avatars/dhalsim.c
			FistBlue/avatars/mbison.c
			FistBlue/avatars/sagat.c
			FistBlue/avatars/balrog.c
			FistBlue/avatars/vega.c
			FistBlue/avatars/blanka_comp.c
			FistBlue/avatars/chunli_comp.c
			FistBlue/avatars/ehonda_comp.c
			FistBlue/avatars/guile_comp.c
			FistBlue/avatars/ryuken_comp.c
			FistBlue/avatars/blanka_human.c
			FistBlue/avatars/chunli_human.c
			FistBlue/avatars/ehonda_human.c
			FistBlue/avatars/guile_human.c
			FistBlue/avatars/ryuken_human.c				
)

target_include_directories(glutBasics PUBLIC 
    .
    SiennaBird
    RedHammer 
    FistBlue 
    FistBlue/ai
    FistBlue/actions 
    FistBlue/avatars 
    FistBlue/gfxdata
    FistBlue/scrolls 
    FistBlue/tests
    SwiftBeam
)

target_link_libraries(glutBasics PRIVATE m OpenGL::GL OpenGL::GLU GLUT::GLUT Threads::Threads)

if (APPLE)
    message(STATUS "Designed by Apple in California")
    add_compile_definitions(GL_SILENCE_DEPRECATION=1)
endif()

if (CPS)
    message(FATAL_ERROR "Building for CPS is not supported yet")
endif()

if (NOT UNIX)
    message(FATAL_ERROR "Only Unix builds are supported")
endif()
