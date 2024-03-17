use std::usize;

use crate::{
    assets::load_textures::TextureAtlasMaterial,
    mesh::chunk::{BlockFace, MeshInfo},
};
use bevy::{
    asset::Assets,
    ecs::system::{Commands, Query, Res, ResMut},
    log::info,
    math::{IVec3, UVec2, UVec3, Vec3},
    pbr::{PbrBundle, StandardMaterial},
    render::{
        color::Color,
        mesh::{Indices, Mesh, PrimitiveTopology},
        render_asset::RenderAssetUsages,
    },
    transform::components::Transform,
    utils::default,
};
use rand::seq::SliceRandom;

use crate::world::region::Region;

use super::region::WATER_LEVEL;

pub const CHUNK_SIZE: usize = 32;

#[derive(PartialEq, Eq, Hash)]
pub struct Chunk {
    is_empty: bool,
    position: IVec3, // Chunks are always going to be whole numbers
    blocks: [[[u8; CHUNK_SIZE]; CHUNK_SIZE]; CHUNK_SIZE], // Will probably convert this to 1D if
                     // issues occur
}

impl Chunk {
    pub fn spawn_chunk(
        &self,
        region: &Region,
        commands: &mut Commands,
        meshes: &mut ResMut<Assets<Mesh>>,
        atlases: &Res<TextureAtlasMaterial>,
    ) {
        if self.is_empty {
            return;
        }
        commands.spawn(PbrBundle {
            mesh: meshes.add(self.generate_chunk_mesh(region)),
            material: atlases.block_atlas.clone(),
            transform: Transform::from_xyz(
                self.position.x as f32 * CHUNK_SIZE as f32,
                self.position.y as f32 * CHUNK_SIZE as f32,
                self.position.z as f32 * CHUNK_SIZE as f32,
            ),
            ..default()
        });
    }

    pub fn generate_chunk_mesh(&self, region: &Region) -> Mesh {
        // get block data
        let mut chunk_mesh_data = MeshInfo {
            positions: Vec::new(),
            colours: Vec::new(),
            indices: Vec::new(),
            normals: Vec::new(),
            uvs: Vec::new(),
        };
        for y in 0..CHUNK_SIZE {
            for z in 0..CHUNK_SIZE {
                for x in 0..CHUNK_SIZE {
                    if self.blocks[y][z][x] == 0 {
                        continue;
                    }
                    let tmp = crate::mesh::chunk::generate_block_faces(
                        self.is_visible(
                            UVec3 {
                                x: x as u32,
                                y: y as u32,
                                z: z as u32,
                            },
                            region,
                        ),
                        1.0,
                        self.blocks[y][z][x],
                        Vec3 {
                            x: x as f32,
                            y: y as f32,
                            z: z as f32,
                        },
                    );
                    let indices_offset = chunk_mesh_data.positions.len();
                    chunk_mesh_data.indices.extend(
                        tmp.indices
                            .iter()
                            .map(|x| x + indices_offset as u32)
                            .collect::<Vec<u32>>(),
                    );
                    chunk_mesh_data.uvs.extend(tmp.uvs);
                    chunk_mesh_data.normals.extend(tmp.normals);
                    chunk_mesh_data.positions.extend(tmp.positions);
                    chunk_mesh_data.colours.extend(tmp.colours);
                }
            }
        }

        // Keep the mesh data accessible in future frames for mutating
        Mesh::new(
            PrimitiveTopology::TriangleList,
            RenderAssetUsages::MAIN_WORLD | RenderAssetUsages::RENDER_WORLD,
        )
        .with_inserted_attribute(Mesh::ATTRIBUTE_POSITION, chunk_mesh_data.positions)
        .with_inserted_attribute(Mesh::ATTRIBUTE_NORMAL, chunk_mesh_data.normals)
        .with_inserted_attribute(Mesh::ATTRIBUTE_UV_0, chunk_mesh_data.uvs)
        .with_inserted_indices(Indices::U32(chunk_mesh_data.indices))
    }

    /// Checks surrounding blocks to determine if the block is visible, returning all block faces
    /// that are visible
    /// # Returns
    /// `Some(Vec<BlockFace>)` One or more blocks are touching air or a transparent block
    /// `None` No blocks are touching air or a transparent block
    pub fn is_visible(&self, local_pos: UVec3, parent_region: &Region) -> Option<Vec<BlockFace>> {
        let mut faces: Vec<BlockFace> = Vec::new();
        let x = local_pos.x as usize;
        let y = local_pos.y as usize;
        let z = local_pos.z as usize;
        // Time for some fun conditions
        match self.blocks[y][z].get(x + 1) {
            Some(b) => {
                if *b == 0 {
                    faces.push(BlockFace::POSX);
                }
            }
            None => faces.push(BlockFace::POSX), // Currently just add face but later cull between
                                                 // chunks
        }
        if x > 0 {
            match self.blocks[y][z].get(x - 1) {
                Some(b) => {
                    if *b == 0 {
                        faces.push(BlockFace::NEGX);
                    }
                }
                None => faces.push(BlockFace::NEGX), // Currently just add face but later cull between
                                                     // chunks
            }
        } else {
            faces.push(BlockFace::NEGX);
        }
        match self.blocks[y].get(z + 1) {
            Some(b) => {
                if b[x] == 0 {
                    faces.push(BlockFace::POSZ);
                }
            }
            None => faces.push(BlockFace::POSZ), // Currently just add face but later cull between
                                                 // chunks
        }
        if z > 0 {
            match self.blocks[y].get(z - 1) {
                Some(b) => {
                    if b[x] == 0 {
                        faces.push(BlockFace::NEGZ);
                    }
                }
                None => faces.push(BlockFace::NEGZ), // Currently just add face but later cull between
                                                     // chunks
            }
        } else {
            faces.push(BlockFace::NEGZ);
        }
        match self.blocks.get(y + 1) {
            Some(b) => {
                if b[z][x] == 0 {
                    faces.push(BlockFace::POSY);
                }
            }
            None => faces.push(BlockFace::POSY), // Currently just add face but later cull between
                                                 // chunks
        }
        if y > 0 {
            match self.blocks.get(y - 1) {
                Some(b) => {
                    if b[z][x] == 0 {
                        faces.push(BlockFace::NEGY);
                    }
                }
                None => faces.push(BlockFace::NEGY), // Currently just add face but later cull between
                                                     // chunks
            }
        } else {
            faces.push(BlockFace::NEGY);
        }
        if faces.len() > 0 {
            Some(faces)
        } else {
            None
        }
    }
}

pub fn get_block_type(
    y: isize,
    height: isize,
    chunk_y: isize,
    surface_level: isize,
    is_surface_block: bool,
    is_surface_chunk: bool,
) -> u8 {
    let actual_height = height + (chunk_y * CHUNK_SIZE as isize);
    let actual_y = y + (chunk_y * CHUNK_SIZE as isize);
    if is_surface_chunk {
        if (y >= height) && actual_height > surface_level {
            0 // Air
        } else if ((y) < height - 3) || (actual_y >= 120) && (y < height) {
            2 // Stone
        } else if (y == height) && (actual_height <= WATER_LEVEL as isize) {
            4 // Sand
        } else if is_surface_block {
            1 // Grass
        } else if (y >= height - 4) && (y < height) && (actual_height == surface_level) {
            3 // Dirt
        } else if (y > height) && (actual_y <= WATER_LEVEL as isize) {
            5 // Water
        } else {
            0 // Air
        }
    } else {
        2
    }
}

pub fn generate_chunk(pos: IVec3, region: &Region) -> Chunk {
    let mut blocks = [[[0; CHUNK_SIZE]; CHUNK_SIZE]; CHUNK_SIZE];
    let mut is_empty = true;
    let is_surface_chunk = region.is_surface_chunk(pos.y as isize);
    for x in 0..CHUNK_SIZE {
        for z in 0..CHUNK_SIZE {
            let h = region.get_block_height(
                pos.y as isize,
                UVec2 {
                    x: x as u32,
                    y: z as u32,
                },
            );
            for y in 0..CHUNK_SIZE {
                let block_pos = IVec3 {
                    x: x as i32,
                    y: y as i32,
                    z: z as i32,
                };
                blocks[y][z][x] = get_block_type(
                    y as isize,
                    h,
                    pos.y as isize,
                    region.get_region_height(UVec2 {
                        x: x as u32,
                        y: z as u32,
                    }),
                    region.is_surface_block(block_pos, pos.y),
                    is_surface_chunk,
                );
                if blocks[y][z][x] != 0 {
                    is_empty = false;
                }
            }
        }
    }
    Chunk {
        is_empty,
        position: pos,
        blocks,
    }
}

pub fn generate_test_chunk(pos: IVec3) -> Chunk {
    let mut blocks = [[[0; CHUNK_SIZE]; CHUNK_SIZE]; CHUNK_SIZE];
    let mut rng_thread = rand::thread_rng();
    let block_types: [u8; 2] = [0, 1];
    for y in 0..CHUNK_SIZE {
        for z in 0..CHUNK_SIZE {
            for x in 0..CHUNK_SIZE {
                blocks[y][z][x] = 1;
            }
        }
    }
    dbg!(&blocks);
    Chunk {
        is_empty: false,
        position: pos,
        blocks,
    }
}

pub fn test_spawn_chunk(
    region: &Region,
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    materials: &Res<TextureAtlasMaterial>,
) {
    let test_chunk = generate_test_chunk(IVec3 { x: 0, y: 0, z: 0 });
    test_chunk.spawn_chunk(region, commands, meshes, materials);
}

/*
 * top +y
 * bottom -y
 * right +x
 * left -x
 * back +z
 * worward -z
 */
