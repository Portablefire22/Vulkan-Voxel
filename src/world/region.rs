use crate::world::chunk::*;
use bevy::{
    log::{info, warn},
    math::{IVec2, IVec3, UVec2, Vec2},
    reflect::Array,
    utils::warn,
};
use bracket_noise::prelude::{FastNoise, FractalType, NoiseType};
use std::{
    collections::{BTreeMap, HashMap},
    isize, usize,
};

const REGION_HEIGHT: usize = 256;
pub const WATER_LEVEL: isize = 16;

#[derive(PartialEq, Eq, Hash)]
pub struct Region {
    position: IVec2,
    height_map: [[isize; CHUNK_SIZE]; CHUNK_SIZE],
    chunks: BTreeMap<isize, Chunk>,
}

impl Default for Region {
    fn default() -> Self {
        Self {
            position: IVec2 { x: 0, y: 0 },
            height_map: [[0; CHUNK_SIZE]; CHUNK_SIZE],
            chunks: BTreeMap::new(),
        }
    }
}

impl Region {
    pub fn new(position: IVec2) -> Self {
        let mut t = Self {
            position,
            ..Default::default()
        };
        t.generate_height_map();
        t
    }

    pub fn get_chunk(&self, pos_y: isize) -> Option<&Chunk> {
        self.chunks.get(&pos_y)
    }

    /// Generates a chunk, saves it at the given y position, and returns the chunk
    pub fn create_chunk(&mut self, pos_y: isize) {
        let tmp = generate_chunk(
            IVec3 {
                x: self.position.x,
                y: pos_y as i32,
                z: self.position.y,
            },
            self,
        );
        match self.chunks.insert(pos_y, tmp) {
            Some(_) => warn!(
                "Region '{:?}' just overwrote Chunk '{}'",
                self.position, pos_y
            ),
            None => (),
        };
    }

    pub fn generate_height_map(&mut self) {
        let mut noise = FastNoise::seeded(21321);
        noise.set_noise_type(NoiseType::SimplexFractal);
        noise.set_fractal_type(FractalType::FBM);
        noise.set_fractal_octaves(8);
        noise.set_fractal_gain(0.5);
        noise.set_fractal_lacunarity(2.0);
        noise.set_frequency(0.0005);
        for z in 0..CHUNK_SIZE {
            for x in 0..CHUNK_SIZE {
                let height = REGION_HEIGHT as f32
                    * noise.get_noise(
                        (self.position.x as f32 * CHUNK_SIZE as f32) + (x as f32),
                        (self.position.y as f32 * CHUNK_SIZE as f32) + (z as f32),
                    );
                self.height_map[z][x] = height.floor().abs() as isize;
            }
        }
    }

    pub fn is_surface_block(&self, block_pos: IVec3, chunk_y: i32) -> bool {
        self.height_map[block_pos.z as usize][block_pos.x as usize]
            == (block_pos.y + (chunk_y * CHUNK_SIZE as i32)) as isize
    }

    pub fn is_surface_chunk(&self, pos_y: isize) -> bool {
        for z in 0..self.height_map.len() {
            for x in 0..self.height_map[z].len() {
                if self.height_map[z][x] / CHUNK_SIZE as isize <= pos_y {
                    return true;
                }
            }
        }
        false
    }

    pub fn get_region_height(&self, block_pos: UVec2) -> isize {
        self.height_map[block_pos.y as usize][block_pos.x as usize]
    }

    pub fn get_block_height(&self, chunk_y: isize, block_pos: UVec2) -> isize {
        let mut tmp = self.height_map[block_pos.y as usize][block_pos.x as usize];
        if tmp - (chunk_y * CHUNK_SIZE as isize).abs() <= 0 {
            tmp = 0;
        } else if tmp - (chunk_y * CHUNK_SIZE as isize).abs() >= CHUNK_SIZE as isize {
            tmp = CHUNK_SIZE as isize;
        } else {
            tmp -= (chunk_y * CHUNK_SIZE as isize).abs();
        }

        tmp
    }
}
