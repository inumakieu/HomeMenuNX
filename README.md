<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better, please fork the repo and create a pull request or simply open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
***
***
***
*** To avoid retyping too much info. Do a search and replace for the following:
*** github_username, repo_name, twitter_handle, email
-->





<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/5H4D0WILA/HomeMenuNX">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">HomeMenuNX</h3>

  <p align="center">
    A WIP Home Menu replacement for the Nintendo Switch, inspired by XorTrolls uLaunch.
    <br />
    <a href="https://github.com/5H4D0WILA/HomeMenuNX"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/5H4D0WILA/HomeMenuNX">View Demo</a>
    ·
    <a href="https://github.com/5H4D0WILA/HomeMenuNX/issues">Report Bug</a>
    ·
    <a href="https://github.com/5H4D0WILA/HomeMenuNX/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Credits](#credits)



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

HomeMenuNX tries to give the user a simple yet powerful design that feels intuitive to use, while also trying to be as customizable as possible.



### Built With

* [devkitPro]()
* [libNX]()
* [SDL2]()



<!-- GETTING STARTED -->
## Getting Started

You can either download the latest release, which is tested and guaranteed to work, or compile it from source yourself.
If you want to compile for yourself, please follow the instructions below.

### Prerequisites

You need devkitPro in order to compile this, along with libnx and SDL2.
To install all of them, follow the instructions for installing devkitPro from the official installation guide at: https://github.com/devkitPro/pacman/releases/tag/v1.0.2
After you install (dpk-)pacman successfully, run the command below to install the sdl2 dependencies
```sh
sudo dkp-pacman -S switch-sdl2 switch-sdl2_gfx switch-sdl2_image switch-sdl2_mixer switch-sdl2_net switch-sdl2_ttf
```
Please also make sure that you have the build-essentials (make etc.) to avoid any errors.

### Compile

1. Clone the repo
```sh
git clone https://github.com/5H4D0WILA/HomeMenuNX.git
```
2. run make to compile
```sh
make
```



<!-- USAGE EXAMPLES -->
## Usage

To use this replacement, just copy the .nro file to your switch folder and run it through hb_menuhttps://github.com/5H4D0WILA/HomeMenuNX



<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/5H4D0WILA/HomeMenuNX/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Your Name - [@twitter_handle](https://twitter.com/twitter_handle) - email

Project Link: [https://github.com/5H4D0WILA/HomeMenuNX](https://github.com/5H4D0WILA/HomeMenuNX)



<!-- CREDITS -->
## Credits

Without these people this project would be impossible.

* [XorTroll](https://github.com/XorTroll) - For giving me the inspiration to create this and helping me when i am stuck
* [MasaGratoR](https://github.com/masagrator) - also for helping me when i am stuck and giving me the code for getting the titles installed on the system
* [devkitPro](https://devkitpro.org/) - without the team of devkitPro it would be impossible for projects like these to exist





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo.svg?style=flat-square
[contributors-url]: https://github.com/github_username/repo/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo.svg?style=flat-square
[forks-url]: https://github.com/github_username/repo/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo.svg?style=flat-square
[stars-url]: https://github.com/github_username/repo/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo.svg?style=flat-square
[issues-url]: https://github.com/github_username/repo/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo.svg?style=flat-square
[license-url]: https://github.com/github_username/repo/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/github_username
[product-screenshot]: images/screenshot.png
