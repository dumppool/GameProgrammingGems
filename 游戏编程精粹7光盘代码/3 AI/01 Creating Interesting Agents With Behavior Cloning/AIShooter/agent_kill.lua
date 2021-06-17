function setupFeatureMap()
  AIShooter.addInterval ("my_hp", 0, -math.huge, 5);
  AIShooter.addInterval ("my_hp", 1, 5, 10);
  AIShooter.addInterval ("my_hp", 2, 10, 20);
  AIShooter.addInterval ("my_hp", 3, 20, 40);
  AIShooter.addInterval ("my_hp", 4, 40, 80);
  AIShooter.addInterval ("my_hp", 5, 80, math.huge);
  AIShooter.addInterval ("opp_ddir_from", 0, -math.huge, -1);
  AIShooter.addInterval ("opp_ddir_from", 1, -1, -0.9);
  AIShooter.addInterval ("opp_ddir_from", 2, -0.9, -0.8);
  AIShooter.addInterval ("opp_ddir_from", 3, -0.8, -0.7);
  AIShooter.addInterval ("opp_ddir_from", 4, -0.7, -0.6);
  AIShooter.addInterval ("opp_ddir_from", 5, -0.6, -0.5);
  AIShooter.addInterval ("opp_ddir_from", 6, -0.5, -0.4);
  AIShooter.addInterval ("opp_ddir_from", 7, -0.4, -0.3);
  AIShooter.addInterval ("opp_ddir_from", 8, -0.3, -0.2);
  AIShooter.addInterval ("opp_ddir_from", 9, -0.2, -0.1);
  AIShooter.addInterval ("opp_ddir_from", 10, -0.1, 0);
  AIShooter.addInterval ("opp_ddir_from", 11, 0, 0.1);
  AIShooter.addInterval ("opp_ddir_from", 12, 0.1, 0.2);
  AIShooter.addInterval ("opp_ddir_from", 13, 0.2, 0.3);
  AIShooter.addInterval ("opp_ddir_from", 14, 0.3, 0.4);
  AIShooter.addInterval ("opp_ddir_from", 15, 0.4, 0.5);
  AIShooter.addInterval ("opp_ddir_from", 16, 0.5, 0.6);
  AIShooter.addInterval ("opp_ddir_from", 17, 0.6, 0.7);
  AIShooter.addInterval ("opp_ddir_from", 18, 0.7, 0.8);
  AIShooter.addInterval ("opp_ddir_from", 19, 0.8, 0.9);
  AIShooter.addInterval ("opp_ddir_from", 20, 0.9, 1);
  AIShooter.addInterval ("opp_ddir_from", 21, 1, math.huge);
  AIShooter.addInterval ("opp_ddir_to", 0, -math.huge, -1);
  AIShooter.addInterval ("opp_ddir_to", 1, -1, -0.9);
  AIShooter.addInterval ("opp_ddir_to", 2, -0.9, -0.8);
  AIShooter.addInterval ("opp_ddir_to", 3, -0.8, -0.7);
  AIShooter.addInterval ("opp_ddir_to", 4, -0.7, -0.6);
  AIShooter.addInterval ("opp_ddir_to", 5, -0.6, -0.5);
  AIShooter.addInterval ("opp_ddir_to", 6, -0.5, -0.4);
  AIShooter.addInterval ("opp_ddir_to", 7, -0.4, -0.3);
  AIShooter.addInterval ("opp_ddir_to", 8, -0.3, -0.2);
  AIShooter.addInterval ("opp_ddir_to", 9, -0.2, -0.1);
  AIShooter.addInterval ("opp_ddir_to", 10, -0.1, 0);
  AIShooter.addInterval ("opp_ddir_to", 11, 0, 0.1);
  AIShooter.addInterval ("opp_ddir_to", 12, 0.1, 0.2);
  AIShooter.addInterval ("opp_ddir_to", 13, 0.2, 0.3);
  AIShooter.addInterval ("opp_ddir_to", 14, 0.3, 0.4);
  AIShooter.addInterval ("opp_ddir_to", 15, 0.4, 0.5);
  AIShooter.addInterval ("opp_ddir_to", 16, 0.5, 0.6);
  AIShooter.addInterval ("opp_ddir_to", 17, 0.6, 0.7);
  AIShooter.addInterval ("opp_ddir_to", 18, 0.7, 0.8);
  AIShooter.addInterval ("opp_ddir_to", 19, 0.8, 0.9);
  AIShooter.addInterval ("opp_ddir_to", 20, 0.9, 1);
  AIShooter.addInterval ("opp_ddir_to", 21, 1, math.huge);
  AIShooter.addInterval ("opp_dir_from", 0, -math.huge, -179);
  AIShooter.addInterval ("opp_dir_from", 1, -179, -178);
  AIShooter.addInterval ("opp_dir_from", 2, -178, -177);
  AIShooter.addInterval ("opp_dir_from", 3, -177, -176);
  AIShooter.addInterval ("opp_dir_from", 4, -176, -175);
  AIShooter.addInterval ("opp_dir_from", 5, -175, -174);
  AIShooter.addInterval ("opp_dir_from", 6, -174, -173);
  AIShooter.addInterval ("opp_dir_from", 7, -173, -172);
  AIShooter.addInterval ("opp_dir_from", 8, -172, -171);
  AIShooter.addInterval ("opp_dir_from", 9, -171, -170);
  AIShooter.addInterval ("opp_dir_from", 10, -170, -160);
  AIShooter.addInterval ("opp_dir_from", 11, -160, -150);
  AIShooter.addInterval ("opp_dir_from", 12, -150, -140);
  AIShooter.addInterval ("opp_dir_from", 13, -140, -130);
  AIShooter.addInterval ("opp_dir_from", 14, -130, -120);
  AIShooter.addInterval ("opp_dir_from", 15, -120, -110);
  AIShooter.addInterval ("opp_dir_from", 16, -110, -100);
  AIShooter.addInterval ("opp_dir_from", 17, -100, -90);
  AIShooter.addInterval ("opp_dir_from", 18, -90, -85);
  AIShooter.addInterval ("opp_dir_from", 19, -85, -80);
  AIShooter.addInterval ("opp_dir_from", 20, -80, -75);
  AIShooter.addInterval ("opp_dir_from", 21, -75, -70);
  AIShooter.addInterval ("opp_dir_from", 22, -70, -65);
  AIShooter.addInterval ("opp_dir_from", 23, -65, -60);
  AIShooter.addInterval ("opp_dir_from", 24, -60, -55);
  AIShooter.addInterval ("opp_dir_from", 25, -55, -50);
  AIShooter.addInterval ("opp_dir_from", 26, -50, -45);
  AIShooter.addInterval ("opp_dir_from", 27, -45, -40);
  AIShooter.addInterval ("opp_dir_from", 28, -40, -35);
  AIShooter.addInterval ("opp_dir_from", 29, -35, -30);
  AIShooter.addInterval ("opp_dir_from", 30, -30, -25);
  AIShooter.addInterval ("opp_dir_from", 31, -25, -20);
  AIShooter.addInterval ("opp_dir_from", 32, -20, -15);
  AIShooter.addInterval ("opp_dir_from", 33, -15, -14);
  AIShooter.addInterval ("opp_dir_from", 34, -14, -13);
  AIShooter.addInterval ("opp_dir_from", 35, -13, -12);
  AIShooter.addInterval ("opp_dir_from", 36, -12, -11);
  AIShooter.addInterval ("opp_dir_from", 37, -11, -10);
  AIShooter.addInterval ("opp_dir_from", 38, -10, -9);
  AIShooter.addInterval ("opp_dir_from", 39, -9, -8);
  AIShooter.addInterval ("opp_dir_from", 40, -8, -7);
  AIShooter.addInterval ("opp_dir_from", 41, -7, -6);
  AIShooter.addInterval ("opp_dir_from", 42, -6, -5);
  AIShooter.addInterval ("opp_dir_from", 43, -5, -4);
  AIShooter.addInterval ("opp_dir_from", 44, -4, -3);
  AIShooter.addInterval ("opp_dir_from", 45, -3, -2);
  AIShooter.addInterval ("opp_dir_from", 46, -2, -1);
  AIShooter.addInterval ("opp_dir_from", 47, -1, 0);
  AIShooter.addInterval ("opp_dir_from", 48, 0, 1);
  AIShooter.addInterval ("opp_dir_from", 49, 1, 2);
  AIShooter.addInterval ("opp_dir_from", 50, 2, 3);
  AIShooter.addInterval ("opp_dir_from", 51, 3, 4);
  AIShooter.addInterval ("opp_dir_from", 52, 4, 5);
  AIShooter.addInterval ("opp_dir_from", 53, 5, 6);
  AIShooter.addInterval ("opp_dir_from", 54, 6, 7);
  AIShooter.addInterval ("opp_dir_from", 55, 7, 8);
  AIShooter.addInterval ("opp_dir_from", 56, 8, 9);
  AIShooter.addInterval ("opp_dir_from", 57, 9, 10);
  AIShooter.addInterval ("opp_dir_from", 58, 10, 11);
  AIShooter.addInterval ("opp_dir_from", 59, 11, 12);
  AIShooter.addInterval ("opp_dir_from", 60, 12, 13);
  AIShooter.addInterval ("opp_dir_from", 61, 13, 14);
  AIShooter.addInterval ("opp_dir_from", 62, 14, 15);
  AIShooter.addInterval ("opp_dir_from", 63, 15, 20);
  AIShooter.addInterval ("opp_dir_from", 64, 20, 25);
  AIShooter.addInterval ("opp_dir_from", 65, 25, 30);
  AIShooter.addInterval ("opp_dir_from", 66, 30, 35);
  AIShooter.addInterval ("opp_dir_from", 67, 35, 40);
  AIShooter.addInterval ("opp_dir_from", 68, 40, 45);
  AIShooter.addInterval ("opp_dir_from", 69, 45, 50);
  AIShooter.addInterval ("opp_dir_from", 70, 50, 55);
  AIShooter.addInterval ("opp_dir_from", 71, 55, 60);
  AIShooter.addInterval ("opp_dir_from", 72, 60, 65);
  AIShooter.addInterval ("opp_dir_from", 73, 65, 70);
  AIShooter.addInterval ("opp_dir_from", 74, 70, 75);
  AIShooter.addInterval ("opp_dir_from", 75, 75, 80);
  AIShooter.addInterval ("opp_dir_from", 76, 80, 85);
  AIShooter.addInterval ("opp_dir_from", 77, 85, 90);
  AIShooter.addInterval ("opp_dir_from", 78, 90, 100);
  AIShooter.addInterval ("opp_dir_from", 79, 100, 110);
  AIShooter.addInterval ("opp_dir_from", 80, 110, 120);
  AIShooter.addInterval ("opp_dir_from", 81, 120, 130);
  AIShooter.addInterval ("opp_dir_from", 82, 130, 140);
  AIShooter.addInterval ("opp_dir_from", 83, 140, 150);
  AIShooter.addInterval ("opp_dir_from", 84, 150, 160);
  AIShooter.addInterval ("opp_dir_from", 85, 160, 170);
  AIShooter.addInterval ("opp_dir_from", 86, 170, 171);
  AIShooter.addInterval ("opp_dir_from", 87, 171, 172);
  AIShooter.addInterval ("opp_dir_from", 88, 172, 173);
  AIShooter.addInterval ("opp_dir_from", 89, 173, 174);
  AIShooter.addInterval ("opp_dir_from", 90, 174, 175);
  AIShooter.addInterval ("opp_dir_from", 91, 175, 176);
  AIShooter.addInterval ("opp_dir_from", 92, 176, 177);
  AIShooter.addInterval ("opp_dir_from", 93, 177, 178);
  AIShooter.addInterval ("opp_dir_from", 94, 178, 179);
  AIShooter.addInterval ("opp_dir_from", 95, 179, math.huge);
  AIShooter.addInterval ("opp_dir_to", 0, -math.huge, -179);
  AIShooter.addInterval ("opp_dir_to", 1, -179, -178);
  AIShooter.addInterval ("opp_dir_to", 2, -178, -177);
  AIShooter.addInterval ("opp_dir_to", 3, -177, -176);
  AIShooter.addInterval ("opp_dir_to", 4, -176, -175);
  AIShooter.addInterval ("opp_dir_to", 5, -175, -174);
  AIShooter.addInterval ("opp_dir_to", 6, -174, -173);
  AIShooter.addInterval ("opp_dir_to", 7, -173, -172);
  AIShooter.addInterval ("opp_dir_to", 8, -172, -171);
  AIShooter.addInterval ("opp_dir_to", 9, -171, -170);
  AIShooter.addInterval ("opp_dir_to", 10, -170, -160);
  AIShooter.addInterval ("opp_dir_to", 11, -160, -150);
  AIShooter.addInterval ("opp_dir_to", 12, -150, -140);
  AIShooter.addInterval ("opp_dir_to", 13, -140, -130);
  AIShooter.addInterval ("opp_dir_to", 14, -130, -120);
  AIShooter.addInterval ("opp_dir_to", 15, -120, -110);
  AIShooter.addInterval ("opp_dir_to", 16, -110, -100);
  AIShooter.addInterval ("opp_dir_to", 17, -100, -90);
  AIShooter.addInterval ("opp_dir_to", 18, -90, -85);
  AIShooter.addInterval ("opp_dir_to", 19, -85, -80);
  AIShooter.addInterval ("opp_dir_to", 20, -80, -75);
  AIShooter.addInterval ("opp_dir_to", 21, -75, -70);
  AIShooter.addInterval ("opp_dir_to", 22, -70, -65);
  AIShooter.addInterval ("opp_dir_to", 23, -65, -60);
  AIShooter.addInterval ("opp_dir_to", 24, -60, -55);
  AIShooter.addInterval ("opp_dir_to", 25, -55, -50);
  AIShooter.addInterval ("opp_dir_to", 26, -50, -45);
  AIShooter.addInterval ("opp_dir_to", 27, -45, -40);
  AIShooter.addInterval ("opp_dir_to", 28, -40, -35);
  AIShooter.addInterval ("opp_dir_to", 29, -35, -30);
  AIShooter.addInterval ("opp_dir_to", 30, -30, -25);
  AIShooter.addInterval ("opp_dir_to", 31, -25, -20);
  AIShooter.addInterval ("opp_dir_to", 32, -20, -15);
  AIShooter.addInterval ("opp_dir_to", 33, -15, -14);
  AIShooter.addInterval ("opp_dir_to", 34, -14, -13);
  AIShooter.addInterval ("opp_dir_to", 35, -13, -12);
  AIShooter.addInterval ("opp_dir_to", 36, -12, -11);
  AIShooter.addInterval ("opp_dir_to", 37, -11, -10);
  AIShooter.addInterval ("opp_dir_to", 38, -10, -9);
  AIShooter.addInterval ("opp_dir_to", 39, -9, -8);
  AIShooter.addInterval ("opp_dir_to", 40, -8, -7);
  AIShooter.addInterval ("opp_dir_to", 41, -7, -6);
  AIShooter.addInterval ("opp_dir_to", 42, -6, -5);
  AIShooter.addInterval ("opp_dir_to", 43, -5, -4);
  AIShooter.addInterval ("opp_dir_to", 44, -4, -3);
  AIShooter.addInterval ("opp_dir_to", 45, -3, -2);
  AIShooter.addInterval ("opp_dir_to", 46, -2, -1);
  AIShooter.addInterval ("opp_dir_to", 47, -1, 0);
  AIShooter.addInterval ("opp_dir_to", 48, 0, 1);
  AIShooter.addInterval ("opp_dir_to", 49, 1, 2);
  AIShooter.addInterval ("opp_dir_to", 50, 2, 3);
  AIShooter.addInterval ("opp_dir_to", 51, 3, 4);
  AIShooter.addInterval ("opp_dir_to", 52, 4, 5);
  AIShooter.addInterval ("opp_dir_to", 53, 5, 6);
  AIShooter.addInterval ("opp_dir_to", 54, 6, 7);
  AIShooter.addInterval ("opp_dir_to", 55, 7, 8);
  AIShooter.addInterval ("opp_dir_to", 56, 8, 9);
  AIShooter.addInterval ("opp_dir_to", 57, 9, 10);
  AIShooter.addInterval ("opp_dir_to", 58, 10, 11);
  AIShooter.addInterval ("opp_dir_to", 59, 11, 12);
  AIShooter.addInterval ("opp_dir_to", 60, 12, 13);
  AIShooter.addInterval ("opp_dir_to", 61, 13, 14);
  AIShooter.addInterval ("opp_dir_to", 62, 14, 15);
  AIShooter.addInterval ("opp_dir_to", 63, 15, 20);
  AIShooter.addInterval ("opp_dir_to", 64, 20, 25);
  AIShooter.addInterval ("opp_dir_to", 65, 25, 30);
  AIShooter.addInterval ("opp_dir_to", 66, 30, 35);
  AIShooter.addInterval ("opp_dir_to", 67, 35, 40);
  AIShooter.addInterval ("opp_dir_to", 68, 40, 45);
  AIShooter.addInterval ("opp_dir_to", 69, 45, 50);
  AIShooter.addInterval ("opp_dir_to", 70, 50, 55);
  AIShooter.addInterval ("opp_dir_to", 71, 55, 60);
  AIShooter.addInterval ("opp_dir_to", 72, 60, 65);
  AIShooter.addInterval ("opp_dir_to", 73, 65, 70);
  AIShooter.addInterval ("opp_dir_to", 74, 70, 75);
  AIShooter.addInterval ("opp_dir_to", 75, 75, 80);
  AIShooter.addInterval ("opp_dir_to", 76, 80, 85);
  AIShooter.addInterval ("opp_dir_to", 77, 85, 90);
  AIShooter.addInterval ("opp_dir_to", 78, 90, 100);
  AIShooter.addInterval ("opp_dir_to", 79, 100, 110);
  AIShooter.addInterval ("opp_dir_to", 80, 110, 120);
  AIShooter.addInterval ("opp_dir_to", 81, 120, 130);
  AIShooter.addInterval ("opp_dir_to", 82, 130, 140);
  AIShooter.addInterval ("opp_dir_to", 83, 140, 150);
  AIShooter.addInterval ("opp_dir_to", 84, 150, 160);
  AIShooter.addInterval ("opp_dir_to", 85, 160, 170);
  AIShooter.addInterval ("opp_dir_to", 86, 170, 171);
  AIShooter.addInterval ("opp_dir_to", 87, 171, 172);
  AIShooter.addInterval ("opp_dir_to", 88, 172, 173);
  AIShooter.addInterval ("opp_dir_to", 89, 173, 174);
  AIShooter.addInterval ("opp_dir_to", 90, 174, 175);
  AIShooter.addInterval ("opp_dir_to", 91, 175, 176);
  AIShooter.addInterval ("opp_dir_to", 92, 176, 177);
  AIShooter.addInterval ("opp_dir_to", 93, 177, 178);
  AIShooter.addInterval ("opp_dir_to", 94, 178, 179);
  AIShooter.addInterval ("opp_dir_to", 95, 179, math.huge);
  AIShooter.addInterval ("opp_distance", 0, 0, 0.01);
  AIShooter.addInterval ("opp_distance", 1, 0.01, 0.02);
  AIShooter.addInterval ("opp_distance", 2, 0.02, 0.03);
  AIShooter.addInterval ("opp_distance", 3, 0.03, 0.04);
  AIShooter.addInterval ("opp_distance", 4, 0.04, 0.05);
  AIShooter.addInterval ("opp_distance", 5, 0.05, 0.06);
  AIShooter.addInterval ("opp_distance", 6, 0.06, 0.07);
  AIShooter.addInterval ("opp_distance", 7, 0.07, 0.08);
  AIShooter.addInterval ("opp_distance", 8, 0.08, 0.09);
  AIShooter.addInterval ("opp_distance", 9, 0.09, 0.1);
  AIShooter.addInterval ("opp_distance", 10, 0.1, 0.15);
  AIShooter.addInterval ("opp_distance", 11, 0.15, 0.2);
  AIShooter.addInterval ("opp_distance", 12, 0.2, 0.25);
  AIShooter.addInterval ("opp_distance", 13, 0.25, 0.3);
  AIShooter.addInterval ("opp_distance", 14, 0.3, 0.35);
  AIShooter.addInterval ("opp_distance", 15, 0.35, 0.4);
  AIShooter.addInterval ("opp_distance", 16, 0.4, 0.45);
  AIShooter.addInterval ("opp_distance", 17, 0.45, 0.5);
  AIShooter.addInterval ("opp_distance", 18, 0.5, math.huge);
  AIShooter.addInterval ("opp_hp", 0, -math.huge, 5);
  AIShooter.addInterval ("opp_hp", 1, 5, 10);
  AIShooter.addInterval ("opp_hp", 2, 10, 20);
  AIShooter.addInterval ("opp_hp", 3, 20, 40);
  AIShooter.addInterval ("opp_hp", 4, 40, 80);
  AIShooter.addInterval ("opp_hp", 5, 80, math.huge);
  AIShooter.addInterval ("opp_vnorm", 0, 0, 0.001);
  AIShooter.addInterval ("opp_vnorm", 1, 0.001, 0.0015);
  AIShooter.addInterval ("opp_vnorm", 2, 0.0015, 0.002);
  AIShooter.addInterval ("opp_vnorm", 3, 0.002, 0.0025);
  AIShooter.addInterval ("opp_vnorm", 4, 0.0025, 0.003);
  AIShooter.addInterval ("opp_vnorm", 5, 0.003, 0.0035);
  AIShooter.addInterval ("opp_vnorm", 6, 0.0035, 0.004);
  AIShooter.addInterval ("opp_vnorm", 7, 0.004, 0.0045);
  AIShooter.addInterval ("opp_vnorm", 8, 0.0045, 0.005);
  AIShooter.addInterval ("opp_vnorm", 9, 0.005, math.huge);
end

function turnTree()
  if GameState.opp_dir_to == 44 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 45 then
    Agent.turn("none");
  elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 12 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 34 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 28 then
    Agent.turn("right");
  elseif GameState.opp_dir_to == 41 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 42 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 38 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 39 then
    Agent.turn("none");
  elseif GameState.opp_dir_to >= 29 and GameState.opp_dir_to <= 30 then
    Agent.turn("right");
  elseif GameState.opp_dir_to == 27 then
    Agent.turn("right");
  elseif GameState.opp_dir_to >= 31 and GameState.opp_dir_to <= 32 then
    Agent.turn("left");
  elseif GameState.opp_dir_to == 36 then
    Agent.turn("none");
  elseif GameState.opp_dir_to >= 13 and GameState.opp_dir_to <= 26 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 37 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 46 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 47 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 43 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 35 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 40 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 33 then
    Agent.turn("right");
  elseif GameState.opp_dir_to == 48 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 49 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 50 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 51 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 52 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 53 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 54 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 55 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 56 then
    Agent.turn("right");
  elseif GameState.opp_dir_to == 57 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 58 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 59 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 60 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 61 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 62 then
    Agent.turn("none");
  elseif GameState.opp_dir_to == 63 then
    if GameState.opp_ddir_to >= 15 and GameState.opp_ddir_to <= 17 then
      Agent.turn("none");
    elseif GameState.opp_ddir_to == 19 then
      Agent.turn("none");
    elseif GameState.opp_ddir_to >= 3 and GameState.opp_ddir_to <= 5 then
      Agent.turn("left");
    elseif GameState.opp_ddir_to >= 6 and GameState.opp_ddir_to <= 8 then
      Agent.turn("left");
    elseif GameState.opp_ddir_to >= 0 and GameState.opp_ddir_to <= 2 then
      Agent.turn("left");
    elseif GameState.opp_ddir_to == 18 then
      Agent.turn("none");
    elseif GameState.opp_ddir_to >= 9 and GameState.opp_ddir_to <= 14 then
      Agent.turn("left");
    end
  elseif GameState.opp_dir_to == 64 then
    Agent.turn("right");
  end
end

function accelTree()
  if GameState.opp_ddir_to == 2 then
    Agent.accel("normal");
  elseif GameState.opp_ddir_to == 20 then
    if GameState.opp_dir_to == 60 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 61 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 45 then
      Agent.accel("none");
    elseif GameState.opp_dir_to >= 46 and GameState.opp_dir_to <= 52 then
      Agent.accel("none");
    elseif GameState.opp_dir_to >= 53 and GameState.opp_dir_to <= 59 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 13 then
    Agent.accel("normal");
  elseif GameState.opp_ddir_to == 18 then
    if GameState.opp_dir_from == 27 then
      Agent.accel("normal");
    elseif GameState.opp_dir_from >= 0 and GameState.opp_dir_from <= 26 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 15 then
    if GameState.opp_ddir_from == 11 then
      Agent.accel("normal");
    elseif GameState.opp_ddir_from >= 0 and GameState.opp_ddir_from <= 10 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 10 then
    Agent.accel("none");
  elseif GameState.opp_ddir_to == 16 then
    Agent.accel("normal");
  elseif GameState.opp_ddir_to == 17 then
    if GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 51 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 52 and GameState.opp_dir_to <= 57 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 64 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 58 and GameState.opp_dir_to <= 63 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 5 then
    if GameState.opp_dir_to >= 53 and GameState.opp_dir_to <= 63 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 42 and GameState.opp_dir_to <= 52 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 40 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 41 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 39 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 38 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 1 then
    Agent.accel("normal");
  elseif GameState.opp_ddir_to == 0 then
    if GameState.my_hp >= 0 and GameState.my_hp <= 2 then
      Agent.accel("none");
    elseif GameState.my_hp >= 3 and GameState.my_hp <= 5 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 3 then
    if GameState.opp_dir_to == 44 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 46 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 45 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 42 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 43 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 8 then
    if GameState.opp_distance >= 16 and GameState.opp_distance <= 18 then
      if GameState.opp_dir_to == 33 then
        Agent.accel("normal");
      elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 32 then
        Agent.accel("normal");
      end
    elseif GameState.opp_distance >= 0 and GameState.opp_distance <= 13 then
      Agent.accel("none");
    elseif GameState.opp_distance >= 14 and GameState.opp_distance <= 15 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 9 then
    Agent.accel("none");
  elseif GameState.opp_ddir_to == 14 then
    if GameState.opp_dir_to == 58 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 57 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 59 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 6 then
    Agent.accel("normal");
  elseif GameState.opp_ddir_to == 4 then
    if GameState.opp_dir_to == 42 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 43 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 44 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 40 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 41 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 11 then
    if GameState.opp_dir_from == 29 then
      Agent.accel("none");
    elseif GameState.opp_dir_from == 28 then
      Agent.accel("none");
    elseif GameState.opp_dir_from >= 0 and GameState.opp_dir_from <= 27 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 12 then
    if GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 62 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 63 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 7 then
    Agent.accel("normal");
  elseif GameState.opp_ddir_to == 21 then
    if GameState.opp_dir_to >= 41 and GameState.opp_dir_to <= 42 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 35 and GameState.opp_dir_to <= 36 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 37 and GameState.opp_dir_to <= 38 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 39 and GameState.opp_dir_to <= 40 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 43 and GameState.opp_dir_to <= 44 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 45 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 46 and GameState.opp_dir_to <= 47 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 33 and GameState.opp_dir_to <= 34 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 48 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 49 and GameState.opp_dir_to <= 50 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 51 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 52 and GameState.opp_dir_to <= 53 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 54 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 55 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 56 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 57 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to == 58 then
      Agent.accel("normal");
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 32 then
      Agent.accel("normal");
    end
  elseif GameState.opp_ddir_to == 19 then
    if GameState.opp_dir_from >= 0 and GameState.opp_dir_from <= 26 then
      Agent.accel("normal");
    elseif GameState.opp_dir_from == 27 then
      Agent.accel("normal");
    end
  end
end

function fireTree()
  if GameState.opp_distance == 15 then
    Agent.fire("none")
  elseif GameState.opp_distance == 17 then
    Agent.fire("none")
  elseif GameState.opp_distance == 12 then
    Agent.fire("fire");
  elseif GameState.opp_distance == 16 then
    Agent.fire("none")
  elseif GameState.opp_distance == 13 then
    if GameState.opp_ddir_to >= 0 and GameState.opp_ddir_to <= 8 then
      Agent.fire("fire");
    elseif GameState.opp_ddir_to == 9 then
      if GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 36 then
        Agent.fire("fire");
      elseif GameState.opp_dir_to == 37 then
        Agent.fire("fire");
      end
    elseif GameState.opp_ddir_to == 10 then
      Agent.fire("fire");
    end
  elseif GameState.opp_distance == 18 then
    if GameState.opp_ddir_to == 4 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 3 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 2 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 7 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 6 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 5 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 21 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 1 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 0 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to >= 10 and GameState.opp_ddir_to <= 13 then
      Agent.fire("fire");
    elseif GameState.opp_ddir_to == 20 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to == 19 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to >= 17 and GameState.opp_ddir_to <= 18 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to >= 8 and GameState.opp_ddir_to <= 9 then
      Agent.fire("none")
    elseif GameState.opp_ddir_to >= 14 and GameState.opp_ddir_to <= 16 then
      Agent.fire("none")
    end
  elseif GameState.opp_distance == 14 then
    if GameState.opp_dir_to == 38 then
      Agent.fire("fire");
    elseif GameState.opp_dir_to == 39 then
      Agent.fire("fire");
    elseif GameState.opp_dir_to == 40 then
      Agent.fire("none")
    elseif GameState.opp_dir_to == 41 then
      Agent.fire("none")
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 37 then
      Agent.fire("fire");
    end
  elseif GameState.opp_distance == 11 then
    if GameState.opp_hp >= 0 and GameState.opp_hp <= 4 then
      Agent.fire("fire");
    elseif GameState.opp_hp == 5 then
      if GameState.opp_dir_to == 36 then
        Agent.fire("fire");
      elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 35 then
        Agent.fire("fire");
      end
    end
  elseif GameState.opp_distance >= 5 and GameState.opp_distance <= 10 then
    if GameState.opp_dir_to == 40 then
      Agent.fire("fire");
    elseif GameState.opp_dir_to >= 0 and GameState.opp_dir_to <= 38 then
      Agent.fire("fire");
    elseif GameState.opp_dir_to == 39 then
      if GameState.opp_hp >= 0 and GameState.opp_hp <= 2 then
        Agent.fire("fire");
      elseif GameState.opp_hp == 4 then
        Agent.fire("fire");
      elseif GameState.opp_hp == 3 then
        Agent.fire("fire");
      end
    end
  elseif GameState.opp_distance >= 0 and GameState.opp_distance <= 4 then
    Agent.fire("fire");
  end
end

