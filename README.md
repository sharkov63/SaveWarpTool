# SaveWarpTool
A tool to help simulating warps.

### Supported engines:

  - GoldSource | Most likely all versions
  - Source | Sep 23 2003 (-7) | HL:Source Old Old Engine
  - Source | Apr  6 2006 (2707) | HL2 Old Engine ***UNSTABLE***
  - Source | Dec  3 2009 (4004) | HL2 Old Engine ***UNSTABLE***

### Installation:
  - **All**: Inject library using any injector (it's better to give engine some time to load before injecting).
  - **GoldSource**: rename SWT.dll to DemoPlayer.dll and put in game directory (near hl.exe).
  - **Source**: rename SWT.dll to \<anyname\>.asi and put in **./bin/** directory.

### Usage:
#### Save warping simulation
To simulate a save warp to \<mapname\> **direction** you can use this command:  
`swarp <mapname>`  
Note: For example, if you are at **c0a0** map, and do **swarp c4a1**, you will changelevel to **c0a0a**, not **c4a1**.

#### Wrong warping simulation
You can make a wrong warp on path from <start_map> to <finish_map> using this command:  
`swarp_wrong_rev <start_map> <finish_map>`  
To undo a wrong warp, you can execute this command:  
`swarp_wrong <start_map> <finish_map>`  
**NOTE:** undoing a wrong warp is an "impossible" warp and cannot be done manually.

There is also a command to dump trigger\_changelevel information to console:  
`swarp_dump`   
It prints multiple lines of:  
`"<from_map>" "<landmark>" "<to_map>"`  
means that \<from_map\> have a **trigger\_changelevel** to \<to_map\> using landmark \<landmark\>.

### Waypoints  
Waypoints are used in SaveWarpFinder as potential starting positions.  
`bind <key> swarp_wp_add` - add a waypoint to the list  
`bind <key> swarp_wp_dump` - dump all waypoints in the list to the console  
`bind <key> swarp_wp_clear` - clear the list 

### Additional information
- GoldSource:  
    There is **sv_zmax** cvar which controls draw distance. It autochange to hardcoded into \*.bsp value on every map load. If you are too far from the map in the void, it's difficult to say in which direction the acctual map is located. So, this tool is renaming **sv_zmax** cvar to **swarp_zmax** and changes its value to **65535**.

### Usefull binds:
- Half-Life 1 (including Half-Life: Source)  
  `bind <key> "swarp c5a1"` - save warp forward (l)  
`bind <key> "swarp c0a0"` - save warp backward (k)  
`bind <key> "swarp_wrong_rev c0a0 c5a1"` - wrong warp forward (x)  
`bind <key> "swarp_wrong c0a0 c5a1"` - reverse operation of a forward wrong warp (**ONLY for routing, impossible manually**)  
`bind <key> "swarp_wrong_rev c5a1 c0a0"` - wrong warp backward (v)  
`bind <key> "swarp_wrong c5a1 c0a0"` - reverse operation of a backward wrong warp (**ONLY for routing, impossible manually**)  
- Half-Life: Blue Shift  
  `bind <key> "swarp ba_outro"` - save warp forward (l)  
`bind <key> "swarp ba_tram1"` - save warp backward (k)  
`bind <key> "swarp_wrong_rev ba_tram1 ba_outro"` - wrong warp forward (x)  
`bind <key> "swarp_wrong ba_tram1 ba_outro"` - reverse operation of a forward wrong warp (**ONLY for routing, impossible manually**)  
`bind <key> "swarp_wrong_rev ba_outro ba_tram1"` - wrong warp backward (v)  
`bind <key> "swarp_wrong ba_outro ba_tram1"` - reverse operation of a backward wrong warp (**ONLY for routing, impossible manually**)  
- Half-Life: Opposing Force  
  `bind <key> "swarp of7a0"` - save warp forward (l)  
`bind <key> "swarp of0a0"` - save warp backward (k)  
`bind <key> "swarp_wrong_rev of0a0 of7a0"` - wrong warp forward (x)  
`bind <key> "swarp_wrong of0a0 of7a0"` - reverse operation of a forward wrong warp (**ONLY for routing, impossible manually**)  
`bind <key> "swarp_wrong_rev of7a0 of0a0"` - wrong warp backward (v)  
`bind <key> "swarp_wrong of7a0 of0a0"` - reverse operation of a backward wrong warp (**ONLY for routing, impossible manually**)  
- Half-Life 2  
  `bind <key> "swarp d3_breen_01"` - save warp forward (l)  
`bind <key> "swarp d1_trainstation_01"` - save warp backward (k)  
`bind <key> "swarp_wrong_rev d1_trainstation_01 d3_breen_01"` - wrong warp forward (x)  
`bind <key> "swarp_wrong d1_trainstation_01 d3_breen_01"` - reverse operation of a forward wrong warp (**ONLY for routing, impossible manually**)  
`bind <key> "swarp_wrong_rev d3_breen_01 d1_trainstation_01"` - wrong warp backward (v)  
`bind <key> "swarp_wrong d3_breen_01 d1_trainstation_01"` - reverse operation of a backward wrong warp (**ONLY for routing, impossible manually**)   
