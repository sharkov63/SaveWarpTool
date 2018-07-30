# SaveWarpTool
A tool to simplify switching between maps:
1. without changing position - Save Warp.
2. with changing position relative to landmark - Wrong Save Warp.

### Supported engines:

  - GoldSource | Most likely all versions
  - Source | Sep 23 2003 (-7) | HL:Source Old Old Engine
  - Source | Apr  6 2006 (2707) | HL2 Old Engine ***UNSTABLE***

### Installation:
  - **All**: Inject library using any injector (it's better to give engine some time to load before injecting).
  - **GoldSource**: rename SWT.dll to DemoPlayer.dll and put in game directory (near hl.exe).
  - **Source**: rename SWT.dll to \<anyname\>.asi and put in **./bin/** directory.

### Usage:
To trigger a changelevel2 command in \<mapname\> **direction** you can use this bind:  
`bind <key> "swarp <mapname>"`  

Note: For example, if you are at **c0a0** map, and do **swarp c4a1**, you will changelevel to **c0a0a** not **c4a1**.

There is also a command to dump trigger\_changelevel information to console:  
`swarp_dump`  

It prints multiple lines of:  
`"<from_map>" "<landmark>" "<to_map>"`  
means that \<from_map\> have a **trigger\_changelevel** to \<to_map\> using landmark \<landmark\>.

### Additional information
- GoldSource:  
    There is **sv_zmax** cvar which controls draw distance. It autochange to hardcoded into \*.bsp value on every map load. If you are too far from the map in the void, it's difficult to say in which direction the acctual map is located. So, this tool is renaming **sv_zmax** cvar to **swarp_zmax** and changes its value to **65535**.

### Usefull binds:
- Half-Life 1 (including HL:Source)  
  `bind <key> "swarp c0a0"`  
  `bind <key> "swarp c5a1"`
  `bind <key> "swarp_wrong c0a0 c5a1"`  
  `bind <key> "swarp_wrong_rev c0a0 c5a1"`
- Half-Life: Blue Shift  
  `bind <key> "swarp ba_tram1"`  
  `bind <key> "swarp ba_outro"`
  `bind <key> "swarp_wrong ba_tram1 ba_outro"`  
  `bind <key> "swarp_wrong_rev ba_tram1 ba_outro"`
- Half-Life: Opposing Force  
  `bind <key> "swarp of0a0"`  
  `bind <key> "swarp of7a0"`
  `bind <key> "swarp_wrong of0a0 of7a0"`  
  `bind <key> "swarp_wrong_rev of0a0 of7a0"`
- Half-Life 2
  `bind <key> "swarp d1_trainstation_02"`
  `bind <key> "swarp d3_citadel_05"`
  `bind <key> "swarp_wrong d1_trainstation_02 d3_citadel_05"`
  `bind <key> "swarp_wrong_rev d1_trainstation_02 d3_citadel_05"`
