# BSense, an automated broadband census system
This is one of the tools I developed as part of my PhD at the University of Edinburgh. There isn't much documentation around, but feel free to get in touch by [email](mailto:mino@minux.it)!

Here a short abstract:
> BSense is a flexible broadband mapping system for broadband coverage and quality assessment of broadband connections in a given geographic region. For coverage related analysis, it relies on data that is either obtained from ISPs or generated based on technology
models and information about infrastructure sites. Broadband quality assessment in BSense is via host-based measurements using our multiplatform and technology-adaptive software client which periodically runs as a background process on users’ computers. The host-based software measurement approach employed in BSense is not only cost-effective but is also flexible and reduces measurement bias. BSense also incorporates a flexible broadband quality index for summarizing the collective effect of various underlying attributes such as download/upload speeds and latency.

## Publications:
Please refer to [this publication](https://link.springer.com/chapter/10.1007/978-3-642-30045-5_26):
> Giacomo Bernardi, Damon Fenacci, Mahesh K. Marina and Dimitrios P. Pezaros. *BSense: a flexible and open-source broadband mapping framework.* In proceedings of Networking, May 2012. 

and to Chapter 6 of my thesis, which is available [here](https://www.era.lib.ed.ac.uk/handle/1842/6241?show=full).

Additionally, a few other articles:
- *Technical Report:* Giacomo Bernardi and Mahesh K. Marina. *BSense: a system for enabling automated broadband census.* [pdf](https://minux.it/publications/BSense-techreport.pdf)
- *Conference Paper:* Giacomo Bernardi and Mahesh K. Marina. *BSense: a system for enabling automated broadband census: short paper.* In proceedings of the 4th ACM Workshop on Networked Systems for Developing Regions (NSDR '10), June 2010. [here](http://dl.acm.org/citation.cfm?id=1836010)
- *Whitepaper:* An analysis generated with the BSense system is included in the *Digital Scotland* report presented at the Scottish Parliament in Edinburgh, June 2010. [pdf](https://minux.it/publications/Digital-Scotland.pdf)

## Sample screenshots of BSense:
The initial form used to gather user’s details:
<p align="center"><img src="https://raw.githubusercontent.com/mino98/bsense/master/screenshots/screenshot_form.png"></p>

The agent icon and menu as it appears in Windows, Apple OSX and Linux:
<p align="center"><img src="https://raw.githubusercontent.com/mino98/bsense/master/screenshots/screenshot_menu.png"></p>

Sample webpage showing the measurement results and statistics collected by BSense for a user’s broadband connection when the user clicks on the agent icon and selects the “Show my broadband statistics...” option from the menu:
<p align="center"><img src="https://raw.githubusercontent.com/mino98/bsense/master/screenshots/screenshot_stats.png"></p>

The web-based graphical tool in BSense to assist the administrator in specifying the parameters for each of the SAUs:
<p align="center"><img src="https://raw.githubusercontent.com/mino98/bsense/master/screenshots/GuiUtilityFunction.png"></p>

The geographical web interface of BSense allows third parties (e.g., regulatory bodies and policy makers) to study the data gathered by the system, by visualising it on a map:
<p align="center"><img src="https://raw.githubusercontent.com/mino98/bsense/master/screenshots/screenshot_geo.png"></p>
