##############################################################################
# Copyright (c) 2013-2016, Lawrence Livermore National Security, LLC.
# Produced at the Lawrence Livermore National Laboratory.
#
# This file is part of Spack.
# Created by Todd Gamblin, tgamblin@llnl.gov, All rights reserved.
# LLNL-CODE-647188
#
# For details, see https://github.com/llnl/spack
# Please also see the LICENSE file for our notice and the LGPL.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License (as
# published by the Free Software Foundation) version 2.1, February 1999.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms and
# conditions of the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##############################################################################
#
# This is a template package file for Spack.  We've put "FIXME"
# next to all the things you'll want to change. Once you've handled
# them, you can save this file and test your package like this:
#
#     spack install omniorb
#
# You can edit this file again by typing:
#
#     spack edit omniorb
#
# See the Spack documentation for more information on packaging.
# If you submit this package back to Spack as a pull request,
# please first remove this boilerplate and all FIXME comments.
#
from spack import *

#
# omniORB 4.1.7 de ATT tres legerement retouche pour qu'il puisse s'installe dans 
# un environnement spack (de base il y a un probleme de shebang ...)
#
class OmniorbAnl(AutotoolsPackage):
    """ORB de ATT"""

    homepage = ""
    url      = "omniorb-anl-4.1.7.tar.gz"
    version('4.1.7', sha256='3f42d97cb0344afb25c3b203ec874ad3d2ea944ea75a16bcb5e084c66273691d')
    version('4.3.0')

	# On a besoin de 2.7 <= python < 3.0.0 :
#    depends_on('python@2.7:', type=('link'))
#    depends_on('python@:2.7.16', type=('build', 'link'))
    depends_on('python@:2.7.16', type='run', when="@:4.2.99")
    depends_on('python@3:', type='run', when="@4.3.0:")

    def configure_args(self):
        # FIXME: Add arguments other than --prefix
        # FIXME: If not needed delete this function
        args = []
        return args
