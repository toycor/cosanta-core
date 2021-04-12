Cosanta Core staging tree 0.15
===========================

https://www.cosanta.net/


What is Cosanta?
-------------

Cosanta is an experimental digital ecosystem for provide services B2B, private
payments to anyone, anywhere in the world. Cosanta uses peer-to-peer technology
to operate with no central authority: managing transactions and issuing money
are carried out collectively by the network. Cosanta Core is the name of the open
source software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the Cosanta Core software, see https://www.cosanta.net/.


## Pow / PoS Rewards Breakdown

| Block                 | Reward              | Masternodes | Miners / Stakers   |
|---------------------- |:------------------- |:----------- |:------------------ |
| <11111                | 0.01  COSA + fees   |             |                    |
| <22222                | 0.02  COSA + fees   |             |                    |
| <33333                | 0.03  COSA + fees   |             |                    |
| <44444                | 0.04  COSA + fees   |             |                    |
| <55555                | 0.05  COSA + fees   |             |                    |
| <66666                | 0.06  COSA + fees   |             |                    |
| <77777                | 0.07  COSA + fees   |             |                    |
| <88888                | 0.08  COSA + fees   |             |                    |
| <99999                | 0.09  COSA + fees   |             |                    |
| <111111               | 0.10  COSA + fees   |             |                    |
| <222222               | 0.20  COSA + fees   |             |                    |
| <333333               | 0.30  COSA + fees   |             |                    |
| <444444               | 0.40  COSA + fees   |             |                    |
| <555555               | 0.50  COSA + fees   |             |                    |
| <666666               | 0.60  COSA + fees   |             |                    |
| <700000               |  1    COSA + fees   |             |                    |
| <710000               |  2    COSA + fees   |             |                    |
| <720000               |  3    COSA + fees   |             |                    |
| <730000               |  4    COSA + fees   |             |                    |
| <740000               |  5    COSA + fees   |             |                    |
| <750000               |  6    COSA + fees   |             |                    |
| <760000               |  7    COSA + fees   |             |                    |
| <770000               |  8    COSA + fees   |             |                    |
| <780000               |  9    COSA + fees   |             |                    |
| <790000               | 10    COSA + fees   |             |                    |
| <800000               | 20    COSA + fees   |             |                    |
| <850000               | 30    COSA + fees   |             |                    |
| <900000               | 40    COSA + fees   |             |                    |
|  900000               | 50    COSA + fees   |             |                    |
| 1048576 (0x100000)    | 25    COSA + fees   | 60%         | 40%                |
| 2097152 (0x200000)    | 12.5  COSA + fees   | 60%         | 40%                |
| 3145728 (0x300000)    | 6.25  COSA + fees   | 60%         | 40%                |
| 4194304 (0x400000)    | 3.125 COSA + fees   | 60%         | 40%                |
| ...                   | ...                 | 60%         | 40%                |
| 34603008 (0x2100000)  | ~0    COSA + fees   | 60%         | 40%                |


License
-------

Cosanta Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is meant to be stable. Development is normally done in separate branches.
[Tags](https://github.com/cosanta/cosanta-core/tags) are created to indicate new official,
stable release versions of Cosanta Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Cosanta Core's Transifex page](https://www.transifex.com/projects/p/cosanta/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.

