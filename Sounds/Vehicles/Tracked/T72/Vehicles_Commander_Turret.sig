AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 22
   name "AimTurnTimeCommanderTurret"
   tl -162.678 -1419.835
   children {
    63 111 121
   }
  }
  IOPItemInputClass {
   id 25
   name "AimElevationTimeCommanderTurret"
   tl -168.571 -1135.111
   children {
    63 115
   }
   value 1
  }
  IOPItemInputClass {
   id 37
   name "AimElevationCommanderTurret"
   tl -160 -744.564
   children {
    44 45
   }
   value 30
   valueMin -100
   valueMax 100
  }
  IOPInputValueClass {
   id 43
   name "-1"
   tl 727.262 -828.381
   children {
    45 121
   }
   value -1
  }
  IOPInputValueClass {
   id 116
   name "Dynamic Range [dB]"
   tl -160 -1541.428
   children {
    117
   }
   value -8
  }
 }
 Ops {
  IOPItemOpMulClass {
   id 45
   name "Mul 45"
   tl 957.262 -651.238
   children {
    46
   }
   inputs {
    ConnectionClass connection {
     id 43
     port 0
    }
    ConnectionClass connection {
     id 37
     port 0
    }
   }
  }
  IOPItemOpSumClass {
   id 63
   name "Sum 63"
   tl 951.547 -1393.214
   children {
    27
   }
   inputs {
    ConnectionClass connection {
     id 25
     port 0
    }
    ConnectionClass connection {
     id 22
     port 0
    }
   }
  }
  IOPItemOpInterpolateClass {
   id 111
   name "Interpolate 111"
   tl 279.881 -1417.5
   children {
    118
   }
   inputs {
    ConnectionClass connection {
     id 117
     port 3
    }
    ConnectionClass connection {
     id 22
     port 0
    }
   }
   "X min" 0.5
   "X max" 2
   "Fade In Type" "Power of 1/2"
   "Fade Out Type" "Power of 1/2"
  }
  IOPItemOpInterpolateClass {
   id 115
   name "Interpolate 115"
   tl 287.976 -1142.203
   children {
    119
   }
   inputs {
    ConnectionClass connection {
     id 117
     port 3
    }
    ConnectionClass connection {
     id 25
     port 0
    }
   }
   "X min" 0.5
   "X max" 2
   "Fade In Type" "Power of 1/2"
   "Fade Out Type" "Power of 1/2"
  }
  SignalOpDb2GainClass {
   id 117
   name "Db2Gain 117"
   tl 34.286 -1541.428
   children {
    111 115
   }
   inputs {
    ConnectionClass connection {
     id 116
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 121
   name "Mul 121"
   tl 966.25 -457.917
   children {
    120
   }
   inputs {
    ConnectionClass connection {
     id 22
     port 0
    }
    ConnectionClass connection {
     id 43
     port 0
    }
   }
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 27
   name "Turn+Elev_T"
   tl 1220.357 -1393.469
   input 63
  }
  IOPItemOutputClass {
   id 44
   name "AimElevationTurret"
   tl 1224.643 -763.81
   input 37
  }
  IOPItemOutputClass {
   id 46
   name "minusAimElevationTurret"
   tl 1226.072 -646.693
   input 45
  }
  IOPItemOutputClass {
   id 118
   name "Turn_V"
   tl 1217.143 -1272.857
   input 111
  }
  IOPItemOutputClass {
   id 119
   name "Elev_V"
   tl 1224.286 -1152.857
   input 115
  }
  IOPItemOutputClass {
   id 120
   name "minusAimTurnTimeTurret"
   tl 1216.944 -463.333
   input 121
  }
 }
 compiled IOPCompiledClass {
  visited {
   517 519 389 261 11 262 134 133 391 518 5 651 646 263 390 139 6
  }
  ins {
   IOPCompiledIn {
    data {
     3 65539 131075 327683
    }
   }
   IOPCompiledIn {
    data {
     2 65539 196611
    }
   }
   IOPCompiledIn {
    data {
     2 65538 3
    }
   }
   IOPCompiledIn {
    data {
     2 3 327683
    }
   }
   IOPCompiledIn {
    data {
     1 262147
    }
   }
  }
  ops {
   IOPCompiledOp {
    data {
     1 131074 4 196608 0 131072 0
    }
   }
   IOPCompiledOp {
    data {
     1 2 4 65536 0 0 0
    }
   }
   IOPCompiledOp {
    data {
     1 196610 4 262145 3 0 0
    }
   }
   IOPCompiledOp {
    data {
     1 262146 4 262145 3 65536 0
    }
   }
   IOPCompiledOp {
    data {
     2 131075 196611 2 262144 0
    }
   }
   IOPCompiledOp {
    data {
     1 327682 4 0 0 196608 0
    }
   }
  }
  outs {
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
  }
  processed 17
  version 2
 }
}