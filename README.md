# Pile-up rejection

Version: 4.3

Author: Yige Huang

Date: 24.11.2023

## Quick Start

-

## Change Log

24.11.2023 by yghuang (4.3):

> With new centrality tool / centrality tree / tree reader.
>
> Will use official nTofMatch versus RefMult cut before we do nTofBeta versus RefMult cut.

10.10.2023 by yghuang (4.2):

> Now will read centrality bin edge by default. If it is not specified, just do not use GetCentrality9().

09.10.2023 by yghuang (4.1):

> Now X-axis and Y-axis are exchanged (for better polynomial fit).

05.10.2023 by yghuang (4.0):

> The pile-up rejecton is no longer using the straight lines, instead, will consider the n-sigma of projected distribution.

Aug. 7, 2023 by yghuang (3.0):

> Using new centrlaity util package, see [here](https://github.com/LearnerYme/CentralityUtils.git).

Feb. 7, 2023 by yghuang (2.1):

> Add a SubmitUtils in case you have a big data set (like 19.6).
>
> This can help you split the jobs and merge them then.

Feb. 1, 2023 by yghuang (2.0):

> Using centrality util.

