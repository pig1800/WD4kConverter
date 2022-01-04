# WD4kConverter

A simple Windows command-line tool for changing logical sector size for WD/HGST Datacenter drives.
This program needs administrator privilege to run.
It is designed to work on SATA interface by using ATA Pass-Through function provided by Windows.

# Usage

WD4kConverter \\\\.\\PhysicalDrive[num]

Physical drive numbers can be obtained by Powershell command "Get-Disk | Format-List"

# Attention

Do NOT attempt to run this program on drives already filled with data.
Despite the function itself doesn't affect data directly, but Partition Table and File System are directly relying on sector size.
Your data will remain byte by byte in your drive but your files will be inaccessible!

# Thanks

Bunch of the code is copied from smartmontools.

This page provided a lot of information:

https://superuser.com/questions/1576894/how-to-convert-the-western-digital-ultrastar-dc-hc530-14tb-hdd-from-512e-to-4

Also this page:

https://stackoverflow.com/questions/5070987/sending-ata-commands-directly-to-device-in-windows

Last but not least, the official document from WD themselves:

https://documents.westerndigital.com/content/dam/doc-library/en_us/assets/public/western-digital/product/data-center-drives/ultrastar-dc-hc500-series/product-manual-ultrastar-dc-hc550-sata-oem-spec.pdf

# Disclaimer

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
