AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 1
   name "Speed"
   tl -435 -74
   children {
    3
   }
  }
  IOPInputValueClass {
   id 4
   name "Pitch Min [St]"
   tl -433 27
   children {
    5
   }
   value -6
  }
  IOPInputValueClass {
   id 6
   name "Pitch Max [St]"
   tl -437.5 117.5
   children {
    7
   }
   value -4
  }
 }
 Ops {
  IOPItemOpInterpolateClass {
   id 3
   name "Interpolate 3"
   tl -17 -20
   children {
    2
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
    ConnectionClass connection {
     id 7
     port 4
    }
    ConnectionClass connection {
     id 5
     port 3
    }
   }
   "X min" 20
   "X max" 80
  }
  SignalOpSt2GainClass {
   id 5
   name "St2Gain 5"
   tl -252 30
   children {
    3
   }
   inputs {
    ConnectionClass connection {
     id 4
     port 0
    }
   }
  }
  SignalOpSt2GainClass {
   id 7
   name "St2Gain 5"
   tl -256.5 120.5
   children {
    3
   }
   inputs {
    ConnectionClass connection {
     id 6
     port 0
    }
   }
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 2
   name "P"
   tl 224 -20
   input 3
  }
 }
 compiled IOPCompiledClass {
  visited {
   261 263 133 135 5 7 6
  }
  ins {
   IOPCompiledIn {
    data {
     1 3
    }
   }
   IOPCompiledIn {
    data {
     1 65539
    }
   }
   IOPCompiledIn {
    data {
     1 131075
    }
   }
  }
  ops {
   IOPCompiledOp {
    data {
     1 2 6 0 0 131073 4 65537 3
    }
   }
   IOPCompiledOp {
    data {
     1 3 2 65536 0
    }
   }
   IOPCompiledOp {
    data {
     1 3 2 131072 0
    }
   }
  }
  outs {
   IOPCompiledOut {
    data {
     0
    }
   }
  }
  processed 7
  version 2
 }
}