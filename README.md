# insight

Interactive time-series display &amp; analysis. Configurable &amp; extensible per-user.

We aim to build a software tool that facilitates telemetry inspection during complex system design and optimisation. 

Some demonstrative use cases, based on authors' experience:
 - to examine sensor data in order to investigate a certain physical phenomenon.
 - to examine estimator state and perceived control outputs in order to optimise a controller design.
 - to view system condition & performance metrics live in order to support effective system utilisation.

We aim to make this tool relevant across many industries, so will develop with generality in mind, abstracting the both core functionality (data display) and peripheral operations (data parsing, storage) such that these can be extended with minimal effort, and fit into the same workflow.

Key features:
 - A number of data display forms. Core options will include waveform/time-series display, line and scatter plots. Linked displays update together.
 - Application configuration (display layout, data shown) is described in human-readable format that is portable and easily modifiable. User can save/load configurations that suit their workflow, and maintain a number to suit the different aspects of their role.
 - Extensible data parsing layer. Core CSV parser, custom functions can be implemented to support other file types.
 - Math layer. Math operations computed after data is loaded. New math channels are treated as any other, and can be displayed in the same way.
 - Interface with databasing solution either locally or on common server. Extensible so each user/team can configure/author their own.
 - Live data support.
 
***
Watch this space. We look forward to releasing our first prototype of insight. The concept for this tool was born from the desire for complete control over our workflow(s), and to understand exactly what data is being displayed. 

This will be open-sourced to benefit any who find a use for it: all suggestions/comments/requests are welcome, please reach out to us on either:
 - [kieran.downie@outlook.com](mailto:kieran.downie@outlook.com)
 - [emiliepham95@gmail.com](mailto:emiliepham95@gmail.com)


***
Based on: Qt (v5.15); Qwt (v6.1.5)  
_Check DEPENDENCIES.md for install procedures._
