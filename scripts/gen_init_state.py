#######################################################################################################################
#######################################################################################################################
### HAL9001 Rubix Cube Solving Robot - gen_init_state.py
### Chris Gerth - Summer 2014
###
### Description: Rotates the cube and takes pictures of all six faces. Does basic image recognition on each face to
###              determine the initial state of the cube.
###
#######################################################################################################################
#######################################################################################################################

from PIL import Image
from sets import Set
import os, math

#color defs
CubeColor_undef = -1
CubeColor_red = 0
CubeColor_blue = 1
CubeColor_green = 2
CubeColor_yellow = 3
CubeColor_orange = 4
CubeColor_white = 5


#Colors to Cubie mapping. We use sets since each cubie has a unique collection of colors, but since the orientation
#can change, the order of those colors does not matter. Corner cubies have three colors, edge cubies have two colors.
#We will use the real-world assumption that white is always up,
#red is front, blue is right, green in left, orange is back, and yellow is bottom. When looking at the solved cube, 
#the white-yellow edge is the top row of the white face, and the yellow-red edge is the top row of the yellow face.
#This implies the scan phase will involve a full 360 degree rotation (toward the blue face), startin with the red face, keeping the white 
#face pointed upward. After 360 degrees have been rotated, we rotate 90 degrees up to reveal the top face, and then 
#-180 degrees to image the bottom face, and then +90 degrees to return to the "Home" position. I realize this is a very
#specific mapping, and it can be generalized once the code is working. However, for the initial writing, having the 
#more well-defined structure will be better for debugging.

#ordering of cubie numbering will be as in singmaster notation:

#singmaster def's (edge cubies)
CubieSing_UF = 0
CubieSing_UR = 1
CubieSing_UB = 2
CubieSing_UL = 3
CubieSing_DF = 4
CubieSing_DR = 5
CubieSing_DB = 6
CubieSing_DL = 7
CubieSing_FR = 8
CubieSing_FL = 9
CubieSing_BR = 10
CubieSing_BL = 11
#singmaster def's (corner cubies)
CubieSing_UFR = 0
CubieSing_URB = 1
CubieSing_UBL = 2
CubieSing_ULF = 3
CubieSing_DRF = 4
CubieSing_DFL = 5
CubieSing_DLB = 6
CubieSing_DBR = 7

#singmasteer edge text names (in solved order)
singmaster_edge = ["UF ", "UR ", "UB ", "UL ", "DF ", "DR ", "DB ", "DL ", "FR ", "FL ", "BR ", "BL "]
singmaster_corner = ["UFR ", "URB ", "UBL ", "ULF ", "DRF ", "DFL ", "DLB ", "DBR "]

#Our assumptions thus far imply that the imaging order should be red, blue, orange, green, white, yellow

#This will constrain the robot such that the cube must be loaded in the proper orientation (white facing "up"). A 
#generalization might be to make this "up" color variable, and at each step record the face's color, creating a 
#color-to-face mapping.

#face defs 
CubeFace_front = CubeColor_red 
CubeFace_right = CubeColor_blue 
CubeFace_left = CubeColor_green
CubeFace_down = CubeColor_yellow 
CubeFace_back= CubeColor_orange
CubeFace_up = CubeColor_white

#ordered list of non-ordered sets for corner cubies. singmaster order
corner_cubie_color_sets = [Set([CubeColor_white, CubeColor_red, CubeColor_blue]), Set([CubeColor_white, CubeColor_orange, CubeColor_blue]), Set([CubeColor_white, CubeColor_orange, CubeColor_green]), Set([CubeColor_white, CubeColor_red, CubeColor_green]), Set([CubeColor_yellow, CubeColor_red, CubeColor_blue]), Set([CubeColor_yellow, CubeColor_red, CubeColor_green]), Set([CubeColor_yellow, CubeColor_orange, CubeColor_green]), Set([CubeColor_yellow, CubeColor_blue, CubeColor_orange])]
#     cubie:                         UFR                                                              URB                                                        UBL                                                        ULF                                                 DRF                                                          DFL                                                    DLB                                                    DBR    

#ordered list of non-ordered sets for edge cubies. Singmaster order
edge_cubie_color_sets = [Set([CubeColor_white, CubeColor_red]), Set([CubeColor_white, CubeColor_blue]), Set([CubeColor_white, CubeColor_orange]), Set([CubeColor_white, CubeColor_green]), Set([CubeColor_yellow, CubeColor_red]), Set([CubeColor_yellow, CubeColor_blue]), Set([CubeColor_yellow, CubeColor_orange]), Set([CubeColor_yellow, CubeColor_green]), Set([CubeColor_red, CubeColor_blue]), Set([CubeColor_red, CubeColor_green]), Set([CubeColor_orange, CubeColor_blue]), Set([CubeColor_orange, CubeColor_green])]
#     cubie:                UF                                     UR                                            UB                                   UL                                              DF                                    DR                                     DB                                            DL                                     FR                                    FL                                     BR                                        BL

#create list of lists for storing the cube colors read in by camera (one list per face)
cube_colors = [[CubeColor_undef]*9 for i in range(6)]

#mapping of two/three subfaces in each cubie to index in cube_colors. Singmaster order. 
edge_subface_map = [ [[CubeFace_front, 1], [CubeFace_up, 7]], [[CubeFace_right, 1], [CubeFace_up, 5]], [[CubeFace_back, 1], [CubeFace_up, 1]],[[CubeFace_left, 1], [CubeFace_up, 7]], [[CubeFace_down, 1], [CubeFace_front, 7]], [[CubeFace_down, 5], [CubeFace_right, 7]], [[CubeFace_down, 7], [CubeFace_back, 7]], [[CubeFace_down, 3], [CubeFace_left, 7]],[[CubeFace_front, 5], [CubeFace_right, 3]],[[CubeFace_front, 3], [CubeFace_left, 5]],[[CubeFace_back, 3], [CubeFace_right, 5]],[[CubeFace_back, 5], [CubeFace_left, 5]]]
#cubie:                                  UF                                       UR                                      UB                                      UL                                     DF                                        DR                                           DB                                       DL                                        FR                                        FL                                        BR                                        BL                    \
corner_subface_map = [ [[CubeFace_up, 8], [CubeFace_front, 2],[CubeFace_right, 0]],  [[CubeFace_up, 2], [CubeFace_right, 2],[CubeFace_back, 0]],  [[CubeFace_up, 0], [CubeFace_back, 2],[CubeFace_left, 0]],  [[CubeFace_up, 6], [CubeFace_front, 0],[CubeFace_left, 2]],   [[CubeFace_down, 2], [CubeFace_right, 6],[CubeFace_front, 8]],   [[CubeFace_down, 0], [CubeFace_front, 6],[CubeFace_left, 8]],   [[CubeFace_down, 6], [CubeFace_left, 6],[CubeFace_back, 8]],   [[CubeFace_down, 8], [CubeFace_back, 6],[CubeFace_right, 8]]]
#cubie:                                           UFR                                                    URB                                                         UBL                                                               ULF                                                             DRF                                                             DFL                                                            DLB                                                             DBR                                \

#define pixel locations in cube face images TWEAK THESE TO MATCH REAL-WORLD DATA
top_row_pxl = 50
mid_row_pxl = 140
bot_row_pxl = 230
lft_col_pxl = 50
mid_col_pxl = 130
rgt_col_pxl = 210

#define colors RGB Vals TWEAK THESE TO MATCH REAL-WORLD DATA
red_r = 255
red_g = 0
red_b = 0
blue_r = 0
blue_g = 0
blue_b = 255
green_r = 0
green_g = 255
green_b = 0
yellow_r = 255
yellow_g = 255
yellow_b = 1
orange_r = 255
orange_g = 163
orange_b = 6
white_r = 255
white_g = 255
white_b = 255

#predefine distances
dist_red = 0
dist_blue = 0
dist_green = 0
dist_yellow = 0
dist_orange = 0
dist_white = 0

#define test pixel location vectors. tell the algorithm which pixel to use for color determination for each cubie face
pxl_locs = [[(lft_col_pxl, top_row_pxl),(mid_col_pxl, top_row_pxl),(rgt_col_pxl, top_row_pxl)],
            [(lft_col_pxl, mid_row_pxl),(mid_col_pxl, mid_row_pxl),(rgt_col_pxl, mid_row_pxl)],
            [(lft_col_pxl, bot_row_pxl),(mid_col_pxl, bot_row_pxl),(rgt_col_pxl, bot_row_pxl)]]



#given an acquisition stage, acq_cube_rotate puts the cube in the proper position to be 
#   photographed by the pi camera.
def acq_cube_rotate(stage):
    if stage == 0: #init from load position, prep to  image front
        print "Moving to image front face"
#ADD SERVO MOVING CODE HERE
    elif stage == 1: #prep to image right
        print "Moving to image right face"
#ADD SERVO MOVING CODE HERE
    elif stage == 2: #prep to image back
        print "Moving to image back face"
#ADD SERVO MOVING CODE HERE
    elif stage == 3: #prep to image left
        print "Moving to image left face"
#ADD SERVO MOVING CODE HERE
    elif stage == 4: #prep to image top
        print "Moving to image top face"
#ADD SERVO MOVING CODE HERE
    elif stage == 5: #prep to image bottom
        print "Moving to image bottom face"
#ADD SERVO MOVING CODE HERE
    elif stage == 6: #return to home(?) position
        print "Moving to prep for solving"
#ADD SERVO MOVING CODE HERE

 
    

#Generates an image for each side of the cube
def acquire_imgs():
    for stage_iter in range(0,6):
        acq_cube_rotate(stage_iter); #properly position cube
        cmd = "raspistill -o temp/face"+str(stage_iter)+".jpg -n" #generate command
        os.system(cmd) #take photo
    acq_cube_rotate(6); #return cube to home position

    
    
#######################################################################################################################
#Start of main function (gen_init_state.py)
#######################################################################################################################



#get six images for the Rubik's cube
acquire_imgs() #COMMENT IS TEMP/DEBUG

#open each image, fill the cube_colors array with the selected color
for img_iter in range(0, 6):
    img_path = "../temp/face" + str(img_iter) + ".jpg"
    im = Image.open(img_path)
    rgb_im = im.convert('RGB')
    for x_iter in range(0,3): #iterate over all nine color locations on each face
        for y_iter in range(0,3):
            r, g, b = rgb_im.getpixel(pxl_locs[y_iter][x_iter]) #get pixel value
            #find euclidian distances
            dist_red = math.sqrt( math.pow(r - red_r, 2) + math.pow(g - red_g, 2) + math.pow(b - red_b, 2))
            dist_blue = math.sqrt( math.pow(r - blue_r, 2) + math.pow(g - blue_g, 2) + math.pow(b - blue_b, 2))
            dist_green = math.sqrt( math.pow(r - green_r, 2) + math.pow(g - green_g, 2) + math.pow(b - green_b, 2))
            dist_yellow = math.sqrt( math.pow(r - yellow_r, 2) + math.pow(g - yellow_g, 2) + math.pow(b - yellow_b, 2))
            dist_orange = math.sqrt( math.pow(r - orange_r, 2) + math.pow(g - orange_g, 2) + math.pow(b - orange_b, 2))
            dist_white = math.sqrt( math.pow(r - white_r, 2) + math.pow(g - white_g, 2) + math.pow(b - white_b, 2))
            dist_min = min(dist_red, dist_blue, dist_green, dist_yellow, dist_orange, dist_white) #find minimum distance value
            #TODO - put some threshold catch here to determine if the color doesn't match any of the six
            #figure out which color has that minimum value
            if(dist_min == dist_red):
                color = CubeColor_red
            elif(dist_min == dist_green):
                color = CubeColor_green
            elif(dist_min == dist_blue):
                color = CubeColor_blue
            elif(dist_min == dist_yellow):
                color = CubeColor_yellow
            elif(dist_min == dist_orange):
                color = CubeColor_orange
            else:
                color = CubeColor_white
            #set cubie face as that color
            cube_colors[img_iter][3*y_iter + x_iter] = color
            
print cube_colors # debug

#error check - ensure that every center subface matches the side's solved color in the 
#predefined orientation:
if(CubeFace_front != cube_colors[0][4]):
    print "Error in cube mapping, front face is not correct color"
if(CubeFace_right != cube_colors[1][4]):
    print "Error in cube mapping, front face is not correct color"
if(CubeFace_back != cube_colors[2][4]):
    print "Error in cube mapping, front face is not correct color"
if(CubeFace_left != cube_colors[3][4]):
    print "Error in cube mapping, front face is not correct color"
if(CubeFace_up != cube_colors[4][4]):
    print "Error in cube mapping, front face is not correct color"
if(CubeFace_down != cube_colors[5][4]):
    print "Error in cube mapping, front face is not correct color"

#for each cubie, look at what colors were read in for it, and map it to some cubie value
#edges
assigned_edge_cubies = [-1]*12
#corners
assigned_corner_cubies = [-1]*8

for edge_iter in range(0,12): #iterate over all the edge cubie locations
    for test_iter in range(0,12): #iterate over all the edge cubies we read in
        #compare a cubie's set of subface colors to a set of subface colors actually read in for that cubie. when those sets are the same (ie, the XOR of those sets has zero size), we know we have found a match. 
        edge_cubie_under_test_color_set = Set([cube_colors[edge_subface_map[test_iter][0][0]][edge_subface_map[test_iter][0][1]], cube_colors[edge_subface_map[test_iter][1][0]][edge_subface_map[test_iter][1][1]]])
        if(0 == len(edge_cubie_color_sets[edge_iter].symmetric_difference(edge_cubie_under_test_color_set))):
            assigned_edge_cubies[edge_iter] = test_iter #record that cubie in the location it exists at

for corner_iter in range(0,8): #iterate over all the corner cubie locations
    for test_iter in range(0,8): #iterate over all the corner cubies we read in
        #compare a cubie's set of subface colors to a set of subface colors actually read in for that cubie. when those sets are the same (ie, the XOR of those sets has zero size), we know we have found a match. 
        corner_cubie_under_test_color_set = Set([cube_colors[corner_subface_map[test_iter][0][0]][corner_subface_map[test_iter][0][1]], cube_colors[corner_subface_map[test_iter][1][0]][corner_subface_map[test_iter][1][1]], cube_colors[corner_subface_map[test_iter][2][0]][corner_subface_map[test_iter][2][1]]])
        if(0 == len(corner_cubie_color_sets[corner_iter].symmetric_difference(corner_cubie_under_test_color_set))):
            assigned_corner_cubies[corner_iter] = test_iter
            
print "assigned edge cubies:"
print assigned_edge_cubies
print ""
print "assigned corner cubies:"
print assigned_corner_cubies
print ""

#generate the singmaster representation of the read-in cubie
singmaster_string = ""
for edge_iter in range(0,12):
    if(assigned_edge_cubies[edge_iter] == -1):
        print "ERROR! Edge Cubie not correctly classified"
    singmaster_string = singmaster_string + singmaster_edge[assigned_edge_cubies[edge_iter]]
for corner_iter in range(0,8):
    if(assigned_corner_cubies[corner_iter] == -1):
        print "ERROR! Corner Cubie not correctly classified"
    singmaster_string = singmaster_string + singmaster_corner[assigned_corner_cubies[corner_iter]]

print "Singmaster output:" 
print singmaster_string